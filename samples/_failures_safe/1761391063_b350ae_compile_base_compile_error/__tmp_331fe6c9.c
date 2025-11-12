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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
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
    
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
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
    
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        while (is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
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
            int len = *pos - start;
            if (len > 0 && len < 32) {
                strncpy(token->value, &input[start], len);
                token->value[len] = '\0';
                token->type = TOKEN_STRING;
                return 1;
            }
        }
    }
    return 0;
}

void skip_whitespace(const char *input, int *pos, int *line, int *column) {
    while (input[*pos] != '\0') {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
            (*pos)++;
        } else if (isspace(input[*pos])) {
            (*column)++;
            (*pos)++;
        } else {
            break;
        }
    }
}

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_INVALID, "", *line, *column};
    
    skip_whitespace(input, pos, line, column);
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.column = *column;
    
    if (parse_number(input, pos, &token)) {
        *column += strlen(token.value);
        return token;
    }
    
    if (parse_string(input, pos, &token)) {
        *column += strlen(token.value);
        return token;
    }
    
    if (parse_identifier(input, pos, &token)) {
        *column += strlen(token.value);
        return token;
    }
    
    if (parse_operator(input, pos, &token)) {
        *column += strlen(token.value);
        return token;
    }
    
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*column)++;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF