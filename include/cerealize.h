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


enum json_type {
    JSON_KEY,
    JSON_OBJECT,
    JSON_STRING,
    JSON_INT,
    JSON_REAL,
    JSON_BOOL,
    JSON_NULL,
};

typedef struct json_node {
    char* key;
    union {
        char* string;
        int integer;
        float real;
        char boolean;
        struct json_node* node;
    };
} json_node;

#define JSON_MAX_ERROR_LENGTH 512

typedef struct {
    json_node* nodes;
    unsigned int node_count;
    char* error_text;
    unsigned int error_length;
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
    LEX_QUOTE = '"',
    LEX_COMMA = ',',
    LEX_OPEN_BRACKET = '{',
    LEX_CLOSE_BRACKET = '}',
    LEX_COLON = ':'
};

bool_t is_whitespace(char cur) {
    return (cur == ' ' || cur == '\t' || cur == '\n' || cur == '\r');
}

// bool_t is_number(char cur) {
//     return (cur >= '0' && cur <= '9');
// }

// skip whitespace until next node
void skip_whitespace(const char* json_string, cereal_size_t length, cereal_uint_t* i, bool_t* failure, char* error_text) {
    while (is_whitespace(json_string[*i]) && *i < length) {
        (*i)++;
    }

    if (*i == length) {
        strcat(error_text, "CERIALIZE ERROR: EOF reached during parsing(perhaps missing a closing tag?)\n");
        *failure = TRUE;
    }
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

cereal_size_t get_node_count(const char* tokens, cereal_size_t length, bool_t* failure, char* error_text) {
    // COUNTING - TODO: move counting to separate method
    if (tokens[0] != LEX_OPEN_BRACKET) {
        strcat(error_text, "CERIALIZE ERROR: Missing opening bracket( '{' ).\n");
        *failure = TRUE;
    }

    cereal_uint_t i = 1; // skip opening '{'
    cereal_size_t node_count = 0;
    do {
        if (*failure) {
            break;
        }

        // parse key
        skip_whitespace(tokens, length, &i, failure, error_text);
        json_parse_string(tokens, length, &i, failure, error_text);
        if (*failure) {
            return 0;
        }

        // colon
        skip_whitespace(tokens, length, &i, failure, error_text);
        if (*failure) {
            return 0;
        }
        if (tokens[i] != LEX_COLON) {
            strcat(error_text, "CERIALIZE ERROR: No value found associated with key(missing ':')");
            *failure = TRUE;
            return 0;
        }
        i++;
        
        // value

        // comma


    } while (i++ < length); // skip past closing '}'

    if (tokens[length - 1] != LEX_CLOSE_BRACKET) {
        strcat(error_text, "CERIALIZE ERROR: Missing opening bracket( '}' ).\n");
        *failure = TRUE;
    }
    
    return node_count;
}

// parse json
json parse_json(const char* json_string, cereal_size_t length) {

    bool_t failure = FALSE;
    char error_text[JSON_MAX_ERROR_LENGTH] = "";

    cereal_size_t node_count = get_node_count(json_string, length, &failure, error_text);

    if (failure) {
        json result = {0};
        result.error_text = error_text;
        result.error_length = JSON_MAX_ERROR_LENGTH;
        return result;
    }

    // capture nodes based on tokenization
    // json_node nodes[node_count]; // unused variable, remove to fix warning

    cereal_uint_t i = 1; // skip opening '{'
    do {
        
        // TODO: try to parse key, colon, object, comma

    } while (i++ < length - 1); // skip past closing '}'

    json result = {0};
    // result.nodes = nodes; // nodes not actually populated
    result.node_count = node_count;
    return result;
}

#endif
