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

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (*pos && strchr(DELIMITERS, *pos)) pos++;
        if (!*pos) break;
        
        if (isdigit(*pos)) {
            int len = 0;
            while (isdigit(pos[len]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = pos[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            pos += len;
            token_count++;
        }
        else if (isalpha(*pos) || *pos == '_') {
            int len = 0;
            while ((isalnum(pos[len]) || pos[len] == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = pos[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            pos += len;
            token_count++;
        }
        else if (*pos == '"') {
            int len = 1;
            tokens[token_count].value[0] = '"';
            pos++;
            
            while (*pos && *pos != '"' && len < MAX_TOKEN_LEN - 2) {
                tokens[token_count].value[len] = *pos;
                len++;
                pos++;
            }
            
            if (*pos == '"') {
                tokens[token_count].value[len] = '"';
                len++;
                pos++;
                tokens[token_count].value[len] = '\0';
                tokens[token_count].type = TOKEN_STRING;
                token_count++;
            }
        }
        else if (strchr("+-*/=<>!&|", *pos)) {
            tokens[token_count].value[0] = *pos;
            
            if (pos[1] && strchr("=+-", pos[1]) && 
                ((*pos == '+' && pos[1] == '+') || 
                 (*pos == '-' && pos[1] == '-') ||
                 (*pos == '=' && pos[1] == '=') ||
                 (*pos == '!' && pos[1] == '=') ||
                 (*pos == '<' && pos[1] == '=') ||
                 (*pos == '>' && pos[1] == '=') ||
                 (*pos == '&' && pos[1] == '&') ||
                 (*pos == '|' && pos[1] == '|'))) {
                tokens[token_count].value[1] = pos[1];
                tokens[token_count].value[2] = '\0';
                pos += 2;
            }
            else {
                tokens[token_count].value[1] = '\0';
                pos++;
            }
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
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
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count <= 0) {
        printf("No tokens found\n");
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