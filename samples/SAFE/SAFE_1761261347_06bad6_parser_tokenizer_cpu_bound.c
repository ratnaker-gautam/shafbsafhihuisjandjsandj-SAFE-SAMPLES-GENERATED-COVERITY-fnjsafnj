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
    TOKEN_PUNCTUATION,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isalpha(*ptr) || *ptr == '_') {
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*ptr == '"') {
            int i = 0;
            ptr++;
            while (*ptr != '"' && *ptr && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            if (*ptr == '"') ptr++;
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            int i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (is_punctuation_char(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PUNCTUATION;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
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
        case TOKEN_PUNCTUATION: printf("PUNCTUATION"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[4096];
    printf("Enter text to tokenize: ");
    
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
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" '%s'\n", tokens[i].value);
    }
    
    return 0;
}