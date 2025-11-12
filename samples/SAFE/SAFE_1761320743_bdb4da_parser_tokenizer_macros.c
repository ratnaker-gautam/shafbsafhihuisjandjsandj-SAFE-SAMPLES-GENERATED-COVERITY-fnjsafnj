//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
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

void tokenize(const char* input) {
    token_count = 0;
    current_token = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        const char* start = ptr;
        
        if (isalpha(*ptr) || *ptr == '_') {
            while (isalnum(*ptr) || *ptr == '_') ptr++;
            size_t len = ptr - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            char ident[MAX_TOKEN_LEN];
            strncpy(ident, start, len);
            ident[len] = '\0';
            add_token(TOKEN_IDENTIFIER, ident);
        }
        else if (isdigit(*ptr)) {
            while (isdigit(*ptr)) ptr++;
            size_t len = ptr - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            char num[MAX_TOKEN_LEN];
            strncpy(num, start, len);
            num[len] = '\0';
            add_token(TOKEN_NUMBER, num);
        }
        else if (*ptr == '"') {
            ptr++;
            start = ptr;
            while (*ptr && *ptr != '"') ptr++;
            if (*ptr == '"') {
                size_t len = ptr - start;
                if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
                char str[MAX_TOKEN_LEN];
                strncpy(str, start, len);
                str[len] = '\0';
                add_token(TOKEN_STRING, str);
                ptr++;
            }
        }
        else if (is_operator(*ptr)) {
            char op[2] = {*ptr, '\0'};
            add_token(TOKEN_OPERATOR, op);
            ptr++;
        }
        else {
            char unknown[2] = {*ptr, '\0'};
            add_token(TOKEN_UNKNOWN, unknown);
            ptr++;
        }
    }
    add_token(TOKEN_EOF, "");
}

Token* next_token() {
    if (current_token >= token_count) return NULL;
    return &tokens[current_token++];
}

void print_token(const Token* token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("%-12s: %s\n", type_names[token->type], token->value);
}

int main() {
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
    
    tokenize(input);
    
    printf("\nTokens:\n");
    Token* token;
    while ((token = next_token()) != NULL) {
        print_token(token);
    }
    
    return 0;
}