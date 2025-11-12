//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_tokens(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        if (isalpha(*p) || *p == '_') {
            char *token_ptr = tokens[token_count].value;
            int len = 0;
            
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
                len++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (isdigit(*p)) {
            char *token_ptr = tokens[token_count].value;
            int len = 0;
            
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
                len++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*p == '"') {
            char *token_ptr = tokens[token_count].value;
            int len = 0;
            p++;
            
            while (*p != '"' && *p != '\0' && len < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
                len++;
            }
            *token_ptr = '\0';
            if (*p == '"') p++;
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*p)) {
            char *token_ptr = tokens[token_count].value;
            int len = 0;
            
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
                len++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            p++;
        }
    }
    
    if (token_count < max_tokens) {
        strcpy(tokens[token_count].value, "EOF");
        tokens[token_count].type = TOKEN_EOF;
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> %s\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
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
    
    int token_count = parse_tokens(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}