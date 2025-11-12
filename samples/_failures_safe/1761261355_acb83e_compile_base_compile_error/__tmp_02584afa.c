//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LENGTH 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LENGTH];
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
        if (*pos - start >= MAX_TOKEN_LENGTH - 1) return 0;
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LENGTH) {
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
        while ((isalnum(input[*pos]) || input[*pos] == '_') && (*pos - start < MAX_TOKEN_LENGTH - 1)) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LENGTH) {
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
        if (is_operator_char(input[*pos]) && (*pos - start < MAX_TOKEN_LENGTH - 1)) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LENGTH) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"' && (*pos - start < MAX_TOKEN_LENGTH - 1)) {
        if (input[*pos] == '\\') {
            (*pos)++;
            if (input[*pos] == '\0') return 0;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int len = *pos - start;
        if (len < MAX_TOKEN_LENGTH) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
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
        struct Token token = {0};
        token.line = line;
        token.column = current_column;
        
        int found = 0;
        
        if (parse_string(input, &pos, &token)) {
            found = 1;
        } else if (parse_number(input, &pos, &token)) {
            found = 1;
        } else if (parse_identifier(input, &pos, &token)) {
            found = 1;
        } else if (parse_operator(input, &pos, &token)) {
            found = 1;
        }
        
        if (found) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
                column += (pos - (current_column - 1));
            }
        } else {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else