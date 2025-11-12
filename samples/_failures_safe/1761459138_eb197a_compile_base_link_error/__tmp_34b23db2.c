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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
};

int is_operator_char(char c) {
    const char operators[] = "+-*/=<>!&|";
    for (size_t i = 0; i < strlen(operators); i++) {
        if (c == operators[i]) return 1;
    }
    return 0;
}

int parse_number(const char *input, size_t *pos, char *buffer) {
    size_t start = *pos;
    size_t buf_pos = 0;
    
    while (isdigit(input[*pos])) {
        if (buf_pos < 31) {
            buffer[buf_pos++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, size_t *pos, char *buffer) {
    size_t start = *pos;
    size_t buf_pos = 0;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        if (buf_pos < 31) {
            buffer[buf_pos++] = input[*pos];
        }
        (*pos)++;
        
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (buf_pos < 31) {
                buffer[buf_pos++] = input[*pos];
            }
            (*pos)++;
        }
        
        buffer[buf_pos] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, size_t *pos, char *buffer) {
    size_t buf_pos = 0;
    
    if (is_operator_char(input[*pos])) {
        if (buf_pos < 31) {
            buffer[buf_pos++] = input[*pos];
        }
        (*pos)++;
        
        while (is_operator_char(input[*pos]) && buf_pos < 2) {
            if (buf_pos < 31) {
                buffer[buf_pos++] = input[*pos];
            }
            (*pos)++;
        }
        
        buffer[buf_pos] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, size_t *pos, char *buffer) {
    size_t buf_pos = 0;
    
    if (input[*pos] == '"') {
        (*pos)++;
        
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (buf_pos < 31) {
                buffer[buf_pos++] = input[*pos];
            }
            (*pos)++;
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            buffer[buf_pos] = '\0';
            return 1;
        }
    }
    return 0;
}

void tokenize(const char *input, struct Token *tokens, size_t max_tokens, size_t *token_count) {
    size_t pos = 0;
    size_t line = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            (*token_count)++;
            continue;
        }
        
        if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            (*token_count)++;
            continue;
        }
        
        if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            (*token_count)++;
            continue;
        }
        
        if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            (*token_count)++;
            continue;
        }
        
        token->type = TOKEN_UNKNOWN;
        if (pos < strlen(input)) {
            token->value[0] = input[pos];
            token->value[1] = '\0';
            pos++;
        }
        (*token_count)++;
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        (*token_count)++;
    }
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}