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

TokenHandler get_handler(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return handle_number;
        case TOKEN_IDENTIFIER: return handle_identifier;
        case TOKEN_OPERATOR: return handle_operator;
        case TOKEN_STRING: return handle_string;
        default: return NULL;
    }
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (*ptr == '"') {
            ptr++;
            int i = 0;
            while (*ptr != '"' && *ptr != '\0' && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
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

void process_tokens(Token* tokens, int count) {
    if (tokens == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        TokenHandler handler = get_handler(tokens[i].type);
        if (handler != NULL) {
            handler(&tokens[i]);
        }
    }
}

int main() {
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Token tokens[50];
    int token_count = tokenize(input, tokens, 50);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("Tokens found:\n");
    process_tokens(tokens, token_count);
    
    return 0;
}