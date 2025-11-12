//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

typedef struct {
    const char* input;
    size_t pos;
    size_t len;
} Tokenizer;

void init_tokenizer(Tokenizer* t, const char* input) {
    if (t == NULL || input == NULL) return;
    t->input = input;
    t->pos = 0;
    t->len = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int get_next_token(Tokenizer* t, Token* token) {
    if (t == NULL || token == NULL) return 0;
    
    while (t->pos < t->len && is_whitespace(t->input[t->pos])) {
        t->pos++;
    }
    
    if (t->pos >= t->len) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        return 1;
    }
    
    char current = t->input[t->pos];
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        size_t i = 0;
        while (t->pos < t->len && i < MAX_TOKEN_LEN - 1 && isdigit(t->input[t->pos])) {
            token->value[i++] = t->input[t->pos++];
        }
        token->value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (t->pos < t->len && i < MAX_TOKEN_LEN - 1 && 
               (isalnum(t->input[t->pos]) || t->input[t->pos] == '_')) {
            token->value[i++] = t->input[t->pos++];
        }
        token->value[i] = '\0';
    } else if (is_operator(current)) {
        token->type = TOKEN_OPERATOR;
        size_t i = 0;
        while (t->pos < t->len && i < MAX_TOKEN_LEN - 1 && is_operator(t->input[t->pos])) {
            token->value[i++] = t->input[t->pos++];
        }
        token->value[i] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        size_t i = 0;
        t->pos++;
        while (t->pos < t->len && i < MAX_TOKEN_LEN - 1 && t->input[t->pos] != '"') {
            token->value[i++] = t->input[t->pos++];
        }
        token->value[i] = '\0';
        if (t->pos < t->len && t->input[t->pos] == '"') {
            t->pos++;
        }
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = t->input[t->pos++];
        token->value[1] = '\0';
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

int tokenize_input(const char* input, Token* tokens, size_t max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens == 0) return 0;
    
    Tokenizer tokenizer;
    init_tokenizer(&tokenizer, input);
    
    size_t token_count = 0;
    Token token;
    
    while (token_count < max_tokens && get_next_token(&tokenizer, &token)) {
        if (token.type == TOKEN_EOF) break;
        
        tokens[token_count] = token;
        token_count++;
        
        if (token_count >= max_tokens) break;
    }
    
    return token_count;
}

void print_tokens(const Token* tokens, size_t count) {
    if (tokens == NULL) return;
    
    printf("Token count: %zu\n", count);
    printf("----------------\n");
    
    for (size_t i = 0; i < count; i++) {
        printf("Token %zu: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value=\"%s\"\n", tokens[i].value);
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin)