//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, Token* tokens, int* token_count) {
    const char* p = input;
    *token_count = 0;
    
    while (*p && *token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        Token* token = &tokens[(*token_count)++];
        
        if (isdigit(*p)) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (*p && isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
        } else if (isalpha(*p) || *p == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while (*p && (isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
        } else if (*p == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
            if (*p == '"') p++;
        } else if (is_operator(*p)) {
            token->type = TOKEN_OPERATOR;
            int i = 0;
            while (*p && is_operator(*p) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
        } else {
            p++;
            (*token_count)--;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        (*token_count)++;
    }
}

void print_tokens(const Token* tokens, int token_count) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}