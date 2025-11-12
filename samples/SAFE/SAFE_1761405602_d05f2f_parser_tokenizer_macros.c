//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

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

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    Token* token = &tokens[token_count++];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_ERROR;
}

int tokenize(const char* input) {
    if (!input) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    int in_string = 0;
    
    for (size_t i = 0; input[i] != '\0' && token_count < MAX_TOKENS; i++) {
        char c = input[i];
        
        if (in_string) {
            if (c == '"') {
                buffer[buf_pos] = '\0';
                add_token(TOKEN_STRING, buffer);
                buf_pos = 0;
                in_string = 0;
            } else if (buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = c;
            }
            continue;
        }
        
        if (c == '"') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_token(classify_token(buffer), buffer);
                buf_pos = 0;
            }
            in_string = 1;
            continue;
        }
        
        if (strchr(DELIMITERS, c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_token(classify_token(buffer), buffer);
                buf_pos = 0;
            }
            continue;
        }
        
        if (is_operator(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_token(classify_token(buffer), buffer);
                buf_pos = 0;
            }
            buffer[0] = c;
            buffer[1] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            continue;
        }
        
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = c;
        }
    }
    
    if (buf_pos > 0 && !in_string) {
        buffer[buf_pos] = '\0';
        add_token(classify_token(buffer), buffer);
    }
    
    add_token(TOKEN_EOF, "");
    return 1;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "ERROR";
    }
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s", i, token_type_name(tokens[i].type));
        if (tokens[i].type != TOKEN_EOF) {
            printf(" ('%s')", tokens[i].value);
        }
        printf("\n");
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    token_count = 0;
    current_token = 0;
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    print_tokens();
    
    return 0;
}