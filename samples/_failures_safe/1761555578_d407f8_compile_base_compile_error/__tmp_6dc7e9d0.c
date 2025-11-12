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
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_TOKEN_LEN) return TOKEN_UNKNOWN;
    
    if (isdigit((unsigned char)str[0])) {
        for (size_t i = 0; i < len; i++) {
            if (!isdigit((unsigned char)str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (str[0] == '"' && str[len-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (len == 1 && is_operator(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha((unsigned char)str[0]) || str[0] == '_') {
        for (size_t i = 0; i < len; i++) {
            if (!isalnum((unsigned char)str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return 0;
    
    char* copy = malloc(input_len + 1);
    if (copy == NULL) return 0;
    strcpy(copy, input);
    
    char* token_str = strtok(copy, DELIMITERS);
    token_count = 0;
    
    while (token_str != NULL && token_count < MAX_TOKENS) {
        size_t token_len = strlen(token_str);
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(tokens[token_count].value, token_str, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].type = classify_token(token_str);
            token_count++;
        }
        token_str = strtok(NULL, DELIMITERS);
    }
    
    free(copy);
    return 1;
}

void print_token(const Token* token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Type: %-12s Value: %s\n", type_names[token->type], token->value);
}

Token get_next_token(void) {
    if (current_token >= token_count) {
        Token eof_token = {TOKEN_EOF, ""};
        return eof_token;
    }
    return tokens[current_token++];
}

Token peek_token(void) {
    if (current_token >= token_count) {
        Token eof_token = {TOKEN_EOF, ""};
        return eof_token;
    }
    return tokens[current_token];
}

int parse_expression(void) {
    Token token = get_next_token();
    if (token.type == TOKEN_EOF) return 0;
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        Token next = peek_token();
        if (next.type == TOKEN_OPERATOR) {
            get_next_token();
            return parse_expression();
        }
        return 1;
    }
    
    if (token.type == TOKEN_STRING) {
        return 1;
    }
    
    return 0;
}

int parse_statement(void) {
    Token token = peek_token();
    
    if (token.type == TOKEN_IDENTIFIER) {
        get_next_token();
        Token next = get_next_token();
        if (next.type == TOKEN_OPERATOR && strcmp(next.value, "=") == 0) {
            return parse_expression();
        }
        return 0;
    }
    
    return parse_expression();
}

int main(void) {
    char input[1024];
    
    printf("Enter input to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("%zu: ", i);
        print_token(&tokens[i]);