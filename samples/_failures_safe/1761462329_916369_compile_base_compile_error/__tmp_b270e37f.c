//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_UNKNOWN };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, char *buffer, int buffer_size) {
    int start = *pos;
    int buf_pos = 0;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            if (buf_pos < buffer_size - 1) {
                buffer[buf_pos++] = input[*pos];
            }
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            if (buf_pos < buffer_size - 1) {
                buffer[buf_pos++] = input[*pos];
            }
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (buf_pos < buffer_size) {
        buffer[buf_pos] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
    
    return *pos > start;
}

int parse_string(const char *input, int *pos, char *buffer, int buffer_size) {
    if (input[*pos] != '"') return 0;
    
    int buf_pos = 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            if (buf_pos < buffer_size - 1) {
                buffer[buf_pos++] = input[*pos];
                (*pos)++;
                buffer[buf_pos++] = input[*pos];
                (*pos)++;
            } else {
                break;
            }
        } else {
            if (buf_pos < buffer_size - 1) {
                buffer[buf_pos++] = input[*pos];
                (*pos)++;
            } else {
                break;
            }
        }
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
    }
    
    if (buf_pos < buffer_size) {
        buffer[buf_pos] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
    
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer, int buffer_size) {
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    int buf_pos = 0;
    
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        if (buf_pos < buffer_size - 1) {
            buffer[buf_pos++] = input[*pos];
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (buf_pos < buffer_size) {
        buffer[buf_pos] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
    
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer, int buffer_size) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int buf_pos = 0;
    char first_char = input[*pos];
    
    if (buf_pos < buffer_size - 1) {
        buffer[buf_pos++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        if ((first_char == '&' && input[*pos] == '&') ||
            (first_char == '|' && input[*pos] == '|') ||
            (first_char == '=' && input[*pos] == '=') ||
            (first_char == '!' && input[*pos] == '=') ||
            (first_char == '<' && input[*pos] == '=') ||
            (first_char == '>' && input[*pos] == '=')) {
            if (buf_pos < buffer_size - 1) {
                buffer[buf_pos++] = input[*pos];
                (*pos)++;
            }
        }
    }
    
    if (buf_pos < buffer_size) {
        buffer[buf_pos] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
    
    return 1;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens) {
        while (isspace(input[pos])) {
            if