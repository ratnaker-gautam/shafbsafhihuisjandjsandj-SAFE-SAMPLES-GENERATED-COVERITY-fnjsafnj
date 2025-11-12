//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
} Token;

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token* token) {
    printf("Number: %s\n", token->value);
}

void handle_identifier(const Token* token) {
    printf("Identifier: %s\n", token->value);
}

void handle_operator(const Token* token) {
    printf("Operator: %s\n", token->value);
}

void handle_string(const Token* token) {
    printf("String: %s\n", token->value);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input, Token* tokens, int max_tokens, TokenHandler* handlers) {
    int token_count = 0;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        Token token;
        memset(&token, 0, sizeof(token));
        
        if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*p) && i < 31) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < 31) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (is_operator_char(*p)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*p) && i < 31) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (*p == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            p++;
            while (*p && *p != '"' && i < 31) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
            if (*p == '"') p++;
        }
        else {
            p++;
            continue;
        }
        
        if (token_count < max_tokens) {
            tokens[token_count] = token;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    for (int i = 0; i < token_count; i++) {
        if (handlers[tokens[i].type]) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
    
    return token_count;
}

int main() {
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    TokenHandler handlers[5] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL
    };
    
    Token tokens[50];
    int count = tokenize(input, tokens, 50, handlers);
    
    printf("Total tokens processed: %d\n", count - 1);
    
    return 0;
}