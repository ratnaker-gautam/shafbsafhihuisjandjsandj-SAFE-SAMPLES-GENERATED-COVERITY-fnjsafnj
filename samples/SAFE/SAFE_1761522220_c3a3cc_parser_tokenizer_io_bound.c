//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int len = 0;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    buffer[len] = '\0';
    return 1;
}

int parse_symbol(const char *input, int *pos, char *buffer) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int len = 0;
    while (is_symbol_char(input[*pos]) && len < MAX_TOKEN_LEN - 1) {
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int skip_whitespace(const char *input, int *pos) {
    int skipped = 0;
    while (isspace(input[*pos])) {
        (*pos)++;
        skipped++;
    }
    return skipped;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
        } else if (parse_symbol(input, &pos, token->value)) {
            token->type = TOKEN_SYMBOL;
            token_count++;
        } else {
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

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_SYMBOL: printf("SYMBOL"); break;
        case TOKEN_EOF: printf("EOF"); break;
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
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
    }
    
    return 0;
}