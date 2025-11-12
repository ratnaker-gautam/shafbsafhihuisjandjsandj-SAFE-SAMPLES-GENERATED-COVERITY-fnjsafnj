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

int parse_number(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    
    while (isdigit(input[*pos])) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    buffer[buf_pos] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    
    if (!is_valid_identifier_start(input[*pos])) {
        return 0;
    }
    
    while (is_valid_identifier_char(input[*pos])) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    buffer[buf_pos] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    
    if (!is_operator_char(input[*pos])) {
        return 0;
    }
    
    buffer[buf_pos++] = input[(*pos)++];
    
    if (is_operator_char(input[*pos])) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    buffer[buf_pos] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (buf_pos >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    buffer[buf_pos] = '\0';
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') {
            break;
        }
        
        char buffer[MAX_TOKEN_LEN];
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, buffer)) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_identifier(input, &pos, buffer)) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_operator(input, &pos, buffer)) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_string(input, &pos, buffer)) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else {
            token->type = TOKEN_UNKNOWN;
            if (pos < (int)strlen(input)) {
                token->value[0] = input[pos];
                token->value[1] = '\0';
                pos++;
            }
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] =