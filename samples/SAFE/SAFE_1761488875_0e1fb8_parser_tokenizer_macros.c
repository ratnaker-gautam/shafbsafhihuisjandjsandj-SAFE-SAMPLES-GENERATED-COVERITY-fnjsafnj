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
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize(const char* input, Token* tokens) {
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (is_operator(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (isalpha(*ptr)) {
            int len = 0;
            while (isalnum(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_WORD;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    tokens[token_count].type = TOKEN_END;
    return token_count;
}

void print_tokens(const Token* tokens) {
    for (int i = 0; tokens[i].type != TOKEN_END; i++) {
        const char* type_str = "";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_WORD: type_str = "WORD"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s ('%s')\n", i + 1, type_str, tokens[i].value);
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
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("Found %d tokens:\n", token_count);
    print_tokens(tokens);
    
    return 0;
}