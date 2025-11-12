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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        size_t len = 0;
        
        if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (*p == '"') {
            token.type = TOKEN_STRING;
            p++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            if (*p == '"') p++;
        } else if (is_operator(*p)) {
            token.type = TOKEN_OPERATOR;
            token.value[len++] = *p++;
            if (is_operator(*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else {
            token.value[len++] = *p++;
        }
        
        token.value[len] = '\0';
        if (len > 0) {
            tokens[token_count++] = token;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
}

Token next_token() {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

Token peek_token() {
    if (current_token < token_count) {
        return tokens[current_token];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

void reset_parser() {
    current_token = 0;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("%zu: %s '%s'\n", i + 1, token_type_name(tokens[i].type), tokens[i].value);
    }
    
    printf("\nParsing demonstration:\n");
    reset_parser();
    Token token;
    while ((token = next_token()).type != TOKEN_EOF) {
        printf("Parsed: %s '%s'\n", token_type_name(token.type), token.value);
    }
    
    return 0;
}