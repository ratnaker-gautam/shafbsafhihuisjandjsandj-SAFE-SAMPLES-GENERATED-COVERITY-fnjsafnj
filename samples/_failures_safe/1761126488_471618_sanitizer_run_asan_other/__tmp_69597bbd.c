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

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token* token) {
    printf("NUMBER: %s\n", token->value);
}

void handle_identifier(const Token* token) {
    printf("IDENTIFIER: %s\n", token->value);
}

void handle_operator(const Token* token) {
    printf("OPERATOR: %s\n", token->value);
}

void handle_string(const Token* token) {
    printf("STRING: %s\n", token->value);
}

void handle_unknown(const Token* token) {
    printf("UNKNOWN: %s\n", token->value);
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (str[0] == '+' || str[0] == '-' || str[0] == '*' || 
        str[0] == '/' || str[0] == '=' || str[0] == '<' || 
        str[0] == '>') {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len < 2 || str[len-1] != '"') return TOKEN_UNKNOWN;
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    char buffer[256];
    if (strlen(input) >= sizeof(buffer)) return 0;
    strcpy(buffer, input);
    
    int token_count = 0;
    char* pos = buffer;
    
    while (*pos && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        if (*pos == '\0') break;
        
        char* start = pos;
        while (*pos && !isspace(*pos)) pos++;
        
        size_t len = pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(tokens[token_count].value, start, len);
        tokens[token_count].value[len] = '\0';
        
        tokens[token_count].type = classify_token(tokens[token_count].value);
        token_count++;
        
        if (*pos) pos++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (!tokens || !handlers || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type < 6) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main(void) {
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count <= 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    TokenHandler handlers[6] = {
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