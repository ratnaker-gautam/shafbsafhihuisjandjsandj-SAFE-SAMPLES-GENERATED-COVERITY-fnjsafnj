//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input) {
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        Token* token = &tokens[token_count];
        
        if (isalpha(*ptr) || *ptr == '_') {
            token->type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            token->value[i] = '\0';
            token_count++;
        }
        else if (isdigit(*ptr)) {
            token->type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            token->value[i] = '\0';
            token_count++;
        }
        else if (is_operator(*ptr)) {
            token->type = TOKEN_OPERATOR;
            size_t i = 0;
            while (is_operator(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            token->value[i] = '\0';
            token_count++;
        }
        else if (*ptr == '"') {
            token->type = TOKEN_STRING;
            ptr++;
            size_t i = 0;
            while (*ptr && *ptr != '"' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            if (*ptr == '"') {
                ptr++;
            }
            token->value[i] = '\0';
            token_count++;
        }
        else {
            token->type = TOKEN_ERROR;
            token->value[0] = *ptr++;
            token->value[1] = '\0';
            token_count++;
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input);
    print_tokens();
    
    return 0;
}