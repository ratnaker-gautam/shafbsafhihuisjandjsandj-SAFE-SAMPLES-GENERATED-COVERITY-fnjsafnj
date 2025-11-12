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
    printf("NUMBER: %s\n", token->value);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("IDENTIFIER: %s\n", token->value);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("OPERATOR: %s\n", token->value);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("STRING: %s\n", token->value);
}

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("UNKNOWN: %s\n", token->value);
}

void handle_eof(Token* token) {
    if (token == NULL) return;
    printf("EOF\n");
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos != '\0' && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        
        if (*pos == '\0') break;
        
        if (isdigit(*pos)) {
            int len = 0;
            while (isdigit(*pos) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*pos) || *pos == '_') {
            int len = 0;
            while ((isalnum(*pos) || *pos == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator_char(*pos)) {
            int len = 0;
            while (is_operator_char(*pos) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (*pos == '"') {
            int len = 0;
            tokens[token_count].value[len++] = *pos++;
            while (*pos != '"' && *pos != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            if (*pos == '"') {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else {
            tokens[token_count].value[0] = *pos++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_UNKNOWN) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main(void) {
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = token