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
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
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
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
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
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        while (is_operator_char(input[*pos]) && (*pos - start) < (MAX_TOKEN_LEN - 1)) {
            (*pos)++;
        }
        
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] == '"') {
        (*pos)++;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            if (*pos - start < MAX_TOKEN_LEN) {
                strncpy(token->value, &input[start], *pos - start);
                token->value[*pos - start] = '\0';
                token->type = TOKEN_STRING;
                return 1;
            }
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int line, int column, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = column;
        return 1;
    }
    
    token->line = line;
    token->column = column;
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (!get_next_token(input, &pos, line, column, &tokens[token_count])) {
            break;
        }
        line = tokens[token_count].line;
        column = tokens[token_count].column + (int)strlen(tokens[token_count].value);
        token_count++;