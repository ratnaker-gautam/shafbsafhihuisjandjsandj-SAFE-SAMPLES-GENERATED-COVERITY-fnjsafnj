//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

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
        
        if (isdigit(*p)) {
            int i = 0;
            while (*p && isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[i++] = *p++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        }
        else if (isalpha(*p) || *p == '_') {
            int i = 0;
            while (*p && (isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[i++] = *p++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
        }
        else if (*p == '"') {
            int i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[i++] = *p++;
            }
            if (*p == '"') p++;
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_STRING;
            (*token_count)++;
        }
        else if (is_operator(*p)) {
            tokens[*token_count].value[0] = *p;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
            p++;
        }
        else {
            p++;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "");
        (*token_count)++;
    }
}

void print_tokens(const Token* tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        const char* type_str = "";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %d: %s -> '%s'\n", i, type_str, tokens[i].value);
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}