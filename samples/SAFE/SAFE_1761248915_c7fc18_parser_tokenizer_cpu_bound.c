//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 100

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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*p) || *p == '_') {
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator_char(*p)) {
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (*p == '"') {
            int i = 0;
            p++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else {
            p++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
    token_count++;
    
    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main() {
    char input[4096];
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
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Warning: Input too large, some tokens may be truncated\n");
    }
    
    printf("\nTokenized %d tokens:\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}