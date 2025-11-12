//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    int length = *pos - start;
    if (length > 0 && length < 32) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
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
    }
    
    int length = *pos - start;
    if (length > 0 && length < 32) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        if (is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 0 && length < 32) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
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
            (*pos)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 0 && length < 32) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    
    if (input == NULL || pos == NULL || *pos < 0) {
        return token;
    }
    
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        token.line = *line;
        token.column = *column;
        
        if (isdigit(input[*pos])) {
            if (parse_number(input, pos, &token)) {
                *column += (int)strlen(token.value);
                return token;
            }
        } else if (isalpha(input[*pos]) || input[*pos] == '_') {
            if (parse_identifier(input, pos, &token)) {
                *column += (int)strlen(token.value);
                return token;
            }
        } else if (is_operator_char(input[*pos])) {
            if (parse_operator(input, pos, &token)) {
                *column += (int)strlen(token.value);
                return token;
            }
        } else if (input[*pos] == '"') {
            if (parse_string(input, pos, &token)) {
                *column += (int)strlen(token.value);
                return token;
            }
        } else {
            token.value[0] = input[*pos];
            token.value[1] = '\0';
            (*pos)++;
            (*column)++;
            token.type = TOKEN_ERROR;
            return token;
        }
        
        (*pos)++;
        (*column)++;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type)