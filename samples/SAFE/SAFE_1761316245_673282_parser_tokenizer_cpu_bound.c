//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int len = 0;
            while (isdigit(ptr[len]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = ptr[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            ptr += len;
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            int len = 0;
            while ((isalnum(ptr[len]) || ptr[len] == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = ptr[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            ptr += len;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            int len = 0;
            while (is_operator_char(ptr[len]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = ptr[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            ptr += len;
            token_count++;
        }
        else if (*ptr == '"') {
            int len = 1;
            tokens[token_count].value[0] = '"';
            while (ptr[len] && ptr[len] != '"' && len < MAX_TOKEN_LEN - 2) {
                tokens[token_count].value[len] = ptr[len];
                len++;
            }
            if (ptr[len] == '"') {
                tokens[token_count].value[len] = '"';
                tokens[token_count].value[len + 1] = '\0';
                tokens[token_count].type = TOKEN_STRING;
                ptr += len + 1;
                token_count++;
            } else {
                tokens[token_count].value[0] = '\0';
                tokens[token_count].type = TOKEN_EOF;
                break;
            }
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

void print_tokens(const Token* tokens, int count) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main() {
    char input[4096];
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
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count - 1);
    print_tokens(tokens, token_count);
    
    return 0;
}