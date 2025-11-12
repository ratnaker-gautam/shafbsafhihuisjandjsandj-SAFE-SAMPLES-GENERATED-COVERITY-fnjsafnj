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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

TokenType classify_token(const char* value) {
    if (value == NULL || strlen(value) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    
    if (value[0] == '"' && value[strlen(value)-1] == '"') return TOKEN_STRING;
    
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        int start = pos;
        Token current_token;
        memset(&current_token, 0, sizeof(Token));
        
        if (isdigit(input[pos])) {
            while (pos < len && isdigit(input[pos])) {
                if (pos - start < MAX_TOKEN_LEN - 1) {
                    current_token.value[pos - start] = input[pos];
                }
                pos++;
            }
        } else if (input[pos] == '"') {
            current_token.value[0] = input[pos++];
            int i = 1;
            while (pos < len && input[pos] != '"' && i < MAX_TOKEN_LEN - 2) {
                current_token.value[i++] = input[pos++];
            }
            if (pos < len && input[pos] == '"') {
                current_token.value[i++] = input[pos++];
            }
        } else if (is_operator_char(input[pos])) {
            while (pos < len && is_operator_char(input[pos]) && (pos - start) < MAX_TOKEN_LEN - 1) {
                current_token.value[pos - start] = input[pos];
                pos++;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && (pos - start) < MAX_TOKEN_LEN - 1) {
                current_token.value[pos - start] = input[pos];
                pos++;
            }
        } else {
            if (MAX_TOKEN_LEN > 1) {
                current_token.value[0] = input[pos];
                current_token.value[1] = '\0';
            }
            pos++;
        }
        
        current_token.value[MAX_TOKEN_LEN - 1] = '\0';
        current_token.type = classify_token(current_token.value);
        
        if (strlen(current_token.value) > 0) {
            tokens[token_count] = current_token;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "EOF", MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_UNKNOWN) {
            if (handlers[tokens[i].type] != NULL) {
                handlers[tokens[i].type](&tokens[i]);
            }
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets