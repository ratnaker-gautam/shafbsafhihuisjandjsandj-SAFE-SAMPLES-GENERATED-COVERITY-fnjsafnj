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
    printf("Found number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("Found identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("Found operator: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("Found string: %s\n", token->value);
}

void handle_error(Token* token) {
    printf("Error: Invalid token '%s'\n", token->value);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_ERROR;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0])) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isalnum(str[i])) return TOKEN_ERROR;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        if (str[1] == '\0') return TOKEN_OPERATOR;
        return TOKEN_ERROR;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len < 2 || str[len-1] != '"') return TOKEN_ERROR;
        for (size_t i = 1; i < len-1; i++) {
            if (str[i] == '"') return TOKEN_ERROR;
        }
        return TOKEN_STRING;
    }
    
    return TOKEN_ERROR;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        if (*pos == '\0') break;
        
        const char* start = pos;
        while (*pos && !isspace(*pos)) pos++;
        
        size_t len = pos - start;
        if (len >= MAX_TOKEN_LEN) {
            tokens[token_count].type = TOKEN_ERROR;
            strncpy(tokens[token_count].value, start, MAX_TOKEN_LEN-1);
            tokens[token_count].value[MAX_TOKEN_LEN-1] = '\0';
            token_count++;
            continue;
        }
        
        strncpy(tokens[token_count].value, start, len);
        tokens[token_count].value[len] = '\0';
        tokens[token_count].type = classify_token(tokens[token_count].value);
        token_count++;
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
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_ERROR) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    TokenHandler handlers[] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL,
        handle_error
    };
    
    printf("Token analysis:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}