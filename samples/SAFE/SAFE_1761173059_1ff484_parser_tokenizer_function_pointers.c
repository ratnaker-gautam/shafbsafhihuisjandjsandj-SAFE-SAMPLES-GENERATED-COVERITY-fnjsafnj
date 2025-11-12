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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize(const char* input, Token* tokens, size_t max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens == 0) return 0;
    
    size_t token_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && token_count < max_tokens) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token* current = &tokens[token_count];
        
        if (isdigit(input[i])) {
            current->type = TOKEN_NUMBER;
            size_t j = 0;
            while (i < len && j < 31 && isdigit(input[i])) {
                current->value[j++] = input[i++];
            }
            current->value[j] = '\0';
            token_count++;
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            current->type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while (i < len && j < 31 && (isalnum(input[i]) || input[i] == '_')) {
                current->value[j++] = input[i++];
            }
            current->value[j] = '\0';
            token_count++;
        }
        else if (input[i] == '"') {
            current->type = TOKEN_STRING;
            size_t j = 0;
            i++;
            while (i < len && j < 31 && input[i] != '"') {
                current->value[j++] = input[i++];
            }
            current->value[j] = '\0';
            if (i < len && input[i] == '"') i++;
            token_count++;
        }
        else if (is_operator_char(input[i])) {
            current->type = TOKEN_OPERATOR;
            size_t j = 0;
            while (i < len && j < 31 && is_operator_char(input[i])) {
                current->value[j++] = input[i++];
            }
            current->value[j] = '\0';
            token_count++;
        }
        else {
            i++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, size_t count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL) return;
    
    for (size_t i = 0; i < count; i++) {
        Token* token = &tokens[i];
        if (token->type == TOKEN_EOF) break;
        
        if (token->type >= TOKEN_NUMBER && token->type <= TOKEN_STRING) {
            if (handlers[token->type] != NULL) {
                handlers[token->type](token);
            }
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
    size_t token_count = tokenize(input, tokens, 50);
    
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
    
    printf("Tokens found:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}