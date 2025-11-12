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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL || *pos < 0) return 0;
    
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
    if (input == NULL || pos == NULL || token == NULL || *pos < 0) return 0;
    
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
    if (input == NULL || pos == NULL || token == NULL || *pos < 0) return 0;
    
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        if (is_operator_char(input[*pos])) {
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
    if (input == NULL || pos == NULL || token == NULL || *pos < 0) return 0;
    
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
    
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

struct Token get_next_token(const char *input, int *pos, int line, int column) {
    struct Token token = {TOKEN_INVALID, "", line, column};
    
    if (input == NULL || pos == NULL || *pos < 0) {
        return token;
    }
    
    int i = *pos;
    while (input[i] != '\0' && isspace(input[i])) {
        if (input[i] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        i++;
    }
    
    if (input[i] == '\0') {
        token.type = TOKEN_EOF;
        *pos = i;
        return token;
    }
    
    token.line = line;
    token.column = column;
    
    int success = 0;
    int temp_pos = i;
    
    do {
        if (parse_number(input, &temp_pos, &token)) {
            success = 1;
            break;
        }
        temp_pos = i;
        
        if (parse_identifier(input, &temp_pos, &token)) {
            success = 1;
            break;
        }
        temp_pos = i;
        
        if (parse_operator(input, &temp_pos, &token)) {
            success = 1;
            break;
        }
        temp_pos = i;
        
        if (parse_string(input, &temp_pos, &token)) {
            success = 1;
            break;
        }
    } while (0);
    
    if (success) {
        *pos = temp_pos;
        return token