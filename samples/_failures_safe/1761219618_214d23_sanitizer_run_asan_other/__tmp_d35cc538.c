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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (strlen(str) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (strlen(str) == 1 && is_operator(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0) return 1;
    
    char* copy = malloc(len + 1);
    if (copy == NULL) return 0;
    strcpy(copy, input);
    
    char* saveptr = NULL;
    char* token_str = strtok_r(copy, DELIMITERS, &saveptr);
    
    while (token_str != NULL && token_count < MAX_TOKENS) {
        if (strlen(token_str) < MAX_TOKEN_LEN) {
            TokenType type = classify_token(token_str);
            tokens[token_count].type = type;
            strncpy(tokens[token_count].value, token_str, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        token_str = strtok_r(NULL, DELIMITERS, &saveptr);
    }
    
    free(copy);
    return 1;
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

void print_tokens(void) {
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

Token get_next_token(void) {
    if (current_token >= token_count) {
        Token eof_token = {TOKEN_EOF, ""};
        return eof_token;
    }
    return tokens[current_token++];
}

void reset_parser(void) {
    current_token = 0;
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    print_tokens();
    
    printf("\nParsing tokens sequentially:\n");
    reset_parser();
    Token token;
    while ((token = get_next_token()).type != TOKEN_EOF) {
        printf("Parsed: %s '%s'\n", token_type_name(token.type), token.value);
    }
    
    return 0;
}