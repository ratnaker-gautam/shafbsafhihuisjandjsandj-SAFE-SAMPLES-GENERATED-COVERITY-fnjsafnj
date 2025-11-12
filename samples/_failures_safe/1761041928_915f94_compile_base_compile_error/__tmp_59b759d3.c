//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') (*pos)++;
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        if (*pos - start < MAX_TOKEN_LEN) {
            token->type = TOKEN_STRING;
            strncpy(token->value, &input[start], *pos - start);
            token->value[*pos - start] = '\0';
            return 1;
        }
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || column == NULL || token == NULL) return 0;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = *column;
        return 1;
    }
    
    token->line = line;
    token->column = *column;
    
    if (parse_number(input, pos, token)) {
        *column += strlen(token->value);
        return 1;
    }
    
    if (parse_string(input, pos, token)) {
        *column += strlen(token->value);
        return 1;
    }
    
    if (parse_identifier(input, pos, token)) {
        *column += strlen(token->value);
        return 1;
    }