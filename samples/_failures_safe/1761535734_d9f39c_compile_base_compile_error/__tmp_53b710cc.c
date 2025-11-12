//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, Token *token) {
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

int parse_string(const char *input, int *pos, Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    int len = strlen(input);
    (*pos)++;
    
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) (*pos)++;
        (*pos)++;
    }
    
    if (*pos < len && input[*pos] == '"') {
        int copy_len = *pos - start + 1;
        if (copy_len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], copy_len);
            token->value[copy_len] = '\0';
            token->type = TOKEN_STRING;
            (*pos)++;
            return 1;
        }
    }
    return 0;
}

int parse_operator(const char *input, int *pos, Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    int len = strlen(input);
    (*pos)++;
    
    while (*pos < len && is_operator_char(input[*pos]) && *pos - start < MAX_TOKEN_LEN - 1) {
        (*pos)++;
    }
    
    int copy_len = *pos - start;
    if (copy_len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    int len = strlen(input);
    (*pos)++;
    
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int copy_len = *pos - start;
    if (copy_len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_number(input, &pos, &token)) parsed = 1;
        else if (parse_string(input, &pos, &token)) parsed = 1;
        else if (parse_operator(input, &pos, &token)) parsed = 1;
        else if (parse_identifier(input, &pos, &token)) parsed = 1;
        
        if (parsed) {
            tokens[token_count] = token;
            token_count++;
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main() {
    char input[4096];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input