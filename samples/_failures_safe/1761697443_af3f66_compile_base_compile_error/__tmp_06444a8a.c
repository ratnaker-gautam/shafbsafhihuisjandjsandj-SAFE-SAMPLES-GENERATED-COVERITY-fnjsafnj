//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

int parse_operator(const char *input, int *pos, char *output) {
    int len = 0;
    
    while (is_operator_char(input[*pos]) && len < MAX_TOKEN_LEN - 1) {
        output[len++] = input[(*pos)++];
    }
    output[len] = '\0';
    
    return len > 0;
}

int parse_number(const char *input, int *pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    if (!isdigit(input[*pos])) return 0;
    
    while ((isdigit(input[*pos]) || input[*pos] == '.') && len < MAX_TOKEN_LEN - 1) {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        output[len++] = input[(*pos)++];
    }
    output[len] = '\0';
    
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *output) {
    int len = 0;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    output[len++] = input[(*pos)++];
    
    while ((isalnum(input[*pos]) || input[*pos] == '_') && len < MAX_TOKEN_LEN - 1) {
        output[len++] = input[(*pos)++];
    }
    output[len] = '\0';
    
    return len > 0;
}

int parse_string(const char *input, int *pos, char *output) {
    int len = 0;
    
    if (input[*pos] != '"') return 0;
    
    output[len++] = input[(*pos)++];
    
    while (input[*pos] != '"' && input[*pos] != '\0' && len < MAX_TOKEN_LEN - 1) {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            output[len++] = input[(*pos)++];
        }
        output[len++] = input[(*pos)++];
    }
    
    if (input[*pos] == '"') {
        output[len++] = input[(*pos)++];
        output[len] = '\0';
        return 1;
    }
    
    return 0;
}

void skip_whitespace(const char *input, int *pos) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        if (input[pos] == '\0') break;
        
        int current_column = column;
        int start_pos = pos;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = current_column;
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            pos++;
        }
        
        for (int i = start_pos; i < pos; i++) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return