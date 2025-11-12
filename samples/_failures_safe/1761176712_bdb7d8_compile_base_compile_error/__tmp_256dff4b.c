//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

TokenType classify_token(const char *value) {
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (isalpha(value[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    return TOKEN_INVALID;
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && isdigit(*ptr)) {
        ptr++;
    }
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && is_operator_char(*ptr)) {
        ptr++;
    }
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (*ptr != '"') return 0;
    ptr++;
    
    while (*ptr && *ptr != '"') {
        ptr++;
    }
    
    if (*ptr != '"') return 0;
    ptr++;
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token = {TOKEN_INVALID, ""};
        int success = 0;
        
        if (isdigit(*input)) {
            success = parse_number(&input, &token);
        } else if (isalpha(*input)) {
            success = parse_identifier(&input, &token);
        } else if (is_operator_char(*input)) {
            success = parse_operator(&input, &token);
        } else if (*input == '"') {
            success = parse_string(&input, &token);
        } else {
            token.value[0] = *input;
            token.value[1] = '\0';
            token.type = TOKEN_INVALID;
            input++;
            success = 1;
        }
        
        if (success) {
            tokens[count++] = token;
        } else {
            break;
        }
    }
    
    tokens[count].type = TOKEN_EOF;
    tokens[count].value[0] = '\0';
    return count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    printf("%s", names[type]);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }