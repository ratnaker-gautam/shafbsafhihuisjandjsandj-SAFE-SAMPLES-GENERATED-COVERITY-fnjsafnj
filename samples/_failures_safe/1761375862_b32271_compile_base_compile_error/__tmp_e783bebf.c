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
    TOKEN_ERROR
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

void handle_error(Token* token) {
    if (token == NULL) return;
    printf("Error: Invalid token '%s'\n", token->value);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str == NULL || strlen(str) == 0) return TOKEN_ERROR;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isdigit(str[i])) return TOKEN_ERROR;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_ERROR;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        if (strlen(str) == 1) return TOKEN_OPERATOR;
        return TOKEN_ERROR;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len < 2 || str[len-1] != '"') return TOKEN_ERROR;
        for (size_t i = 1; i < len - 1; i++) {
            if (str[i] == '"') return TOKEN_ERROR;
        }
        return TOKEN_STRING;
    }
    
    return TOKEN_ERROR;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos != '\0' && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        if (*pos == '\0') break;
        
        const char* start = pos;
        size_t token_len = 0;
        
        if (isdigit(*pos)) {
            while (isdigit(*pos) && token_len < MAX_TOKEN_LEN - 1) {
                pos++;
                token_len++;
            }
        } else if (isalpha(*pos) || *pos == '_') {
            while ((isalnum(*pos) || *pos == '_') && token_len < MAX_TOKEN_LEN - 1) {
                pos++;
                token_len++;
            }
        } else if (is_operator_char(*pos)) {
            token_len = 1;
            pos++;
        } else if (*pos == '"') {
            token_len = 1;
            pos++;
            while (*pos != '"' && *pos != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                pos++;
                token_len++;
            }
            if (*pos == '"') {
                token_len++;
                pos++;
            }
        } else {
            token_len = 1;
            pos++;
        }
        
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(tokens[token_count].value, start, token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type < TOKEN_EOF) {
            if (handlers[tokens[i].type] != NULL) {
                handlers[tokens[i].type](&tokens[i]);
            }
        }
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {