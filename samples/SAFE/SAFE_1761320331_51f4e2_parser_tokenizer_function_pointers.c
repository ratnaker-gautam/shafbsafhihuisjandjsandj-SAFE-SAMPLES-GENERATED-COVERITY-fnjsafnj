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

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    if (token == NULL) return;
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("String: %s\n", token->value);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* p = input;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (*p == '\0') break;
        
        Token* current = &tokens[token_count];
        
        if (isdigit(*p)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*p) && i < 31) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (isalpha(*p) || *p == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < 31) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (is_operator_char(*p)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*p) && i < 31) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (*p == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            p++;
            while (*p != '"' && *p != '\0' && i < 31) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            if (*p == '"') p++;
            token_count++;
        }
        else {
            p++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        TokenType type = tokens[i].type;
        if (type >= TOKEN_NUMBER && type <= TOKEN_STRING && handlers[type] != NULL) {
            handlers[type](&tokens[i]);
        }
    }
}

int main() {
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[50];
    int token_count = tokenize(input, tokens, 50);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    TokenHandler handlers[5];
    handlers[TOKEN_NUMBER] = handle_number;
    handlers[TOKEN_IDENTIFIER] = handle_identifier;
    handlers[TOKEN_OPERATOR] = handle_operator;
    handlers[TOKEN_STRING] = handle_string;
    handlers[TOKEN_EOF] = NULL;
    
    printf("Tokens:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}