#ifndef json_h
#define json_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int cereal_size_t;
typedef unsigned int cereal_uint_t;

#define TRUE 1
#define FALSE 0
typedef char bool_t;


typedef enum json_type {
    JSON_OBJECT,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL,
} json_type;

typedef union json_value {
        char* string;
        float number;
        bool_t boolean;
        bool_t is_null;
        struct json_node* nodes;
        cereal_size_t node_count;
} json_value;

typedef struct json_node {
    char* key;
    json_value value;
} json_node;


// need to define json_object that tracks the type, and then moodify the lexer to return this
typedef struct json_object {
    json_type type;
    json_value value;
} json_object;



#define JSON_MAX_ERROR_LENGTH 512

typedef struct {
    json_object root;
    char* error_text;
    cereal_size_t error_length;
    bool_t failure;
} json;

// {
//     "example": {
//         "name": "example",
//         "value": 42,
//         "nested": {
//             "key": "value"
//         }
//     }
// }
// {"example":{"name":"example","value": 42,"nested": {"key": "value"}}}

// json parse_json(const char* json_string);

// implementation

// lexer
enum lex_type {
    LEX_QUOTE = '\'',
    LEX_COMMA = ',',
    LEX_PERIOD = '.',
    LEX_OPEN_BRACKET = '{',
    LEX_CLOSE_BRACKET = '}',
    LEX_OPEN_SQUARE = '[',
    LEX_CLOSE_SQUARE = ']',
    LEX_COLON = ':',
    LEX_F = 'f',
    LEX_T = 't',
    LEX_N = 'n',
};

bool_t is_whitespace(char cur) {
    return (cur == ' ' || cur == '\t' || cur == '\n' || cur == '\r');
}

bool_t is_number(char cur) {
    return (cur >= '0' && cur <= '9');
}

// Accepts a sign or digit as the first character of a number
bool_t is_number_start(char cur) {
    return (cur == '-' || cur == '+' || (cur >= '0' && cur <= '9'));
}

// skip whitespace until next node
void skip_whitespace(const char* json_string, cereal_size_t length, cereal_uint_t* i, bool_t* failure, char* error_text) {
    while (is_whitespace(json_string[*i]) && *i < length) {
        (*i)++;
    }

    // if (*i == length) {
    //     strcat(error_text, "CERIALIZE ERROR: EOF reached during parsing(perhaps missing a closing tag?)\n");
    //     *failure = TRUE;
    // }
}

// string     : array of lex tokens representing string
// length     : length of string given
// i          : current parser index
// failure    : track whether parsing failed
// error_text : error text to append to if the parsing fails
// TODO: add parameter to store key length, maybe return a struct that gives this?  not sure.
char* json_parse_string(const char* json_string, cereal_size_t length, cereal_uint_t* i, bool_t* failure, char* error_text) {

    // opening "'"
    if (json_string[*i] != LEX_QUOTE) {
        strcat(error_text, "CERIALIZE ERROR: Expected quote to open JSON string.\n");
        *failure = TRUE;
        return NULL;
    }
    (*i)++;


    // find length of string so we allocate the proper size
    cereal_size_t str_size = 0;
    bool_t found_end = FALSE;
    cereal_uint_t j = *i;
    do {
        if (j >= length) {
            strcat(error_text, "CEREALIZE ERROR: Expecting closing quote to close JSON string.\n");
            *failure = TRUE;
            return NULL;
        }

        if (json_string[j] == LEX_QUOTE) {
            found_end = TRUE;
        } else {
            str_size++;
        }

    } while(!found_end && j++ < length);

    char* str = (char*)malloc(str_size);
    for (cereal_uint_t j = 0; j < str_size; j++) {
        str[j] = json_string[*i];
        (*i)++;
    }

    // closing "'"
    if (json_string[*i] != LEX_QUOTE) {
        strcat(error_text, "CERIALIZE ERROR: Expected quote to open JSON string.\n");
        *failure = TRUE;
        return NULL;
    }
    (*i)++;

    return str;
}

bool_t json_parse_null(const char* json_string, cereal_uint_t* i, bool_t* failure, char* error_text) {
    // check for "null"
    if (strncmp(&json_string[*i], "null", 4) != 0) {
        strcat(error_text, "CERIALIZE ERROR: Expected 'null' keyword.\n");
        *failure = TRUE;
        return FALSE;;
    }
    *i += 4; // move past "null"
    return TRUE; // return null pointer for JSON null
}

float json_parse_number(const char* json_string, cereal_size_t length, cereal_uint_t* i, bool_t* failure, char* error_text) {
    // check for valid number format
    cereal_uint_t start = *i;
    // Accept optional sign at the start
    if (*i < length && (json_string[*i] == '-' || json_string[*i] == '+')) {
        (*i)++;
    }
    bool_t found_digit = 0;
    while (*i < length && (is_number(json_string[*i]) || json_string[*i] == LEX_PERIOD)) {
        if (is_number(json_string[*i])) found_digit = 1;
        (*i)++;
    }

    if (!found_digit) {
        strcat(error_text, "CERIALIZE ERROR: Expected number.\n");
        *failure = TRUE;
        return 0.0f;
    }

    // extract number substring 
    float value = strtof(&json_string[start], NULL);
    return value;
}

bool_t json_parse_boolean(const char* json_string, cereal_uint_t* i, bool_t* failure, char* error_text) {
    if (strncmp(&json_string[*i], "true", 4) == 0) {
        *i += 4;
        return TRUE;
    } else if (strncmp(&json_string[*i], "false", 5) == 0) {
        *i += 5;
        return FALSE;
    } else {
        strcat(error_text, "CERIALIZE ERROR: Expected 'true' or 'false'.\n");
        *failure = TRUE;
        return FALSE; // default return value
    }
}

json_object parse_json_object(const char* json_string, cereal_size_t length, cereal_uint_t* i, char* error_text, bool_t* failure) {
    skip_whitespace(json_string, length, i, NULL, NULL);

    json_object obj;
    obj.type = JSON_OBJECT;

    char cur = json_string[*i];
    if (cur == LEX_QUOTE) {
        obj.value.string = json_parse_string(json_string, length, (cereal_uint_t*)i,failure, error_text);
        obj.type = JSON_STRING;
        return obj;
    }

    if (is_number_start(cur) || cur == LEX_PERIOD) {
        obj.value.number = json_parse_number(json_string, length, (cereal_uint_t*)i,failure, error_text);
        obj.type = JSON_NUMBER;
        return obj;
    }

    if (cur == LEX_N) {
        obj.value.is_null = json_parse_null(json_string, (cereal_uint_t*)i, failure, error_text);
        obj.type = JSON_NULL;
        return obj;
    }

    if (cur == LEX_T || cur == LEX_F) {
        obj.value.boolean = json_parse_boolean(json_string, (cereal_uint_t*)i,failure, error_text);
        obj.type = JSON_BOOL;
        return obj;
    }
    
    // parse build object
    if (cur != LEX_OPEN_BRACKET) {
        strcat(error_text, "CERIALIZE ERROR: Expected opening bracket '{' for JSON object.\n");
        *failure = TRUE;
        return (json_object){0}; // return empty value on error
    }
    (*i)++; // move past '{'

    skip_whitespace(json_string, length, (cereal_uint_t*)i, failure, error_text);

    json_node* head = NULL;
    cereal_size_t node_count = 0;

    // parse key-value pairs
    while (*i < length) {
        skip_whitespace(json_string, length, (cereal_uint_t*)i, failure, error_text);

        if (json_string[*i] == LEX_CLOSE_BRACKET) {
            (*i)++; // move past '}'
            break; // end of object
        }

        char* key = json_parse_string(json_string, length, (cereal_uint_t*)i, failure, error_text);
        if (key == NULL) {
            strcat(error_text, "CERIALIZE ERROR: Failed to parse key in JSON object.\n");
            *failure = TRUE;
            return (json_object){0}; // return empty value on error
        }

        skip_whitespace(json_string, length, (cereal_uint_t*)i, failure, error_text);
        if (json_string[*i] != LEX_COLON) {
            strcat(error_text, "CERIALIZE ERROR: Expected ':' after key in JSON object.\n");
            *failure = TRUE;
            free(key);
            return (json_object){0}; // return empty value on error
        }
        (*i)++; // move past ':'

        skip_whitespace(json_string, length, (cereal_uint_t*)i, failure, error_text);

        json_object value = parse_json_object(json_string, length, i, error_text, failure);
        if (*failure) {
            strcat(error_text, "CERIALIZE ERROR: Failed to parse value in JSON object.\n");
            free(key);
            return (json_object){0}; // return empty value on error
        }

        // create new node
        json_node* new_node = (json_node*)malloc(sizeof(json_node));
        new_node->key = key;
        new_node->value = value.value; // copy the value

        node_count++;
        head = realloc(head, sizeof(json_node) * node_count);
        if (head == NULL) {
            strcat(error_text, "CERIALIZE ERROR: Failed to allocate memory for JSON object.\n");
            *failure = TRUE;
            free(key);
            return (json_object){0}; // return empty value on error
        }
        head[node_count - 1] = *new_node;
        free(new_node);

        skip_whitespace(json_string, length, (cereal_uint_t*)i, failure, error_text);
        if (json_string[*i] != LEX_COMMA && json_string[*i] != LEX_CLOSE_BRACKET && *i != length) {
            strcat(error_text, "CERIALIZE ERROR: Expected ',' or '}' after key-value pair in JSON object.\n");
            *failure = TRUE;
            free(key);
            return (json_object){0}; // return empty value on error
        }
        (*i)++; // move past ',' or '}'

        skip_whitespace(json_string, length, (cereal_uint_t*)i, failure, error_text);
    }


    // create the json_object
    obj.value.nodes = head; // assign the linked list of nodes
    obj.value.node_count = node_count; // store the number of nodes
    obj.type = JSON_OBJECT;

    return obj;
}

// parse json
json parse_json(const char* json_string, cereal_size_t length) {

    bool_t failure = FALSE;
    char* error_text = malloc(JSON_MAX_ERROR_LENGTH);
    if (error_text == NULL) {
        json result = {
            .root = {0},
            .failure = TRUE,
            .error_text = "CERIALIZE ERROR: Failed to allocate memory for error text.\n",
            .error_length = strlen("CERIALIZE ERROR: Failed to allocate memory for error text.\n")
        };
        return result;
    }

    // TODO: parse json object
    cereal_uint_t i = 0;
    json_object root_value = parse_json_object(json_string, length, &i, error_text, &failure);

    json result = {
        .root = root_value,
        .failure = failure,
        .error_text = error_text
    };

    return result;
}

#endif