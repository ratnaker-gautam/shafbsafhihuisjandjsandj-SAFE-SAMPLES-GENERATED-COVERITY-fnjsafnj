//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
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

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("Unknown: %s\n", token->value);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        Token* current = &tokens[token_count];
        
        if (isdigit(*ptr)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (*ptr == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            if (*ptr == '"') ptr++;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *ptr++;
            current->value[1] = '\0';
            token_count++;
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
        if (type >= 0 && type <= TOKEN_UNKNOWN && handlers[type] != NULL) {
            handlers[type](&tokens[i]);
        }
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("No input provided\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    TokenHandler handlers[TOKEN_UNKNOWN + 1] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL,
        handle_unknown
    };
    
    printf("Tokens:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}