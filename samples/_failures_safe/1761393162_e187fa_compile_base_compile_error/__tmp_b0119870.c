//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int len = strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = strlen(input);
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        if (*pos - start < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], *pos - start);
            token->value[*pos - start] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        }
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = strlen(input);
    
    if (*pos < len && is_operator_char(input[*pos])) {
        (*pos)++;
        while (*pos < len && is_operator_char(input[*pos]) && (*pos - start) < (MAX_TOKEN_LEN - 1)) {
            (*pos)++;
        }
        
        if (*pos - start < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], *pos - start);
            token->value[*pos - start] = '\0';
            token->type = TOKEN_OPERATOR;
            return 1;
        }
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = strlen(input);
    
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        while (*pos < len && input[*pos] != '"') {
            (*pos)++;
        }
        
        if (*pos < len && input[*pos] == '"') {
            int copy_len = *pos - start - 1;
            if (copy_len < MAX_TOKEN_LEN - 1) {
                strncpy(token->value, &input[start + 1], copy_len);
                token->value[copy_len] = '\0';
                token->type = TOKEN_STRING;
                (*pos)++;
                return 1;
            }
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int line, int *column, struct Token *token) {
    int len = strlen(input);
    
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = *column;
        return 1;
    }
    
    token->line = line;
    token->column = *column;
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*column)++;
    return 1;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}