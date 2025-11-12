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
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    while (*pos < input_len && isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    if (*pos >= input_len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int input_len = strlen(input);
    if (*pos >= input_len || !is_operator_char(input[*pos])) return 0;
    
    buffer[0] = input[(*pos)++];
    buffer[1] = '\0';
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int input_len = strlen(input);
    
    while (pos < input_len && token_count < max_tokens) {
        while (pos < input_len && isspace(input[pos])) pos++;
        if (pos >= input_len) break;
        
        char buffer[MAX_TOKEN_LEN];
        struct Token *token = &tokens[token_count];
        
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
        } else if (input[pos] == '(' || input[pos] == ')') {
            token->type = TOKEN_PAREN;
            token->value[0] = input[pos++];
            token->value[1] = '\0';
            token_count++;
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = input[pos++];
            token->value[1] = '\0';
            token_count++;
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PAREN: return "PAREN";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("%-12s %-16s %s\n", "Position", "Type", "Value");
    printf("--------------------------------\n");
    
    for (