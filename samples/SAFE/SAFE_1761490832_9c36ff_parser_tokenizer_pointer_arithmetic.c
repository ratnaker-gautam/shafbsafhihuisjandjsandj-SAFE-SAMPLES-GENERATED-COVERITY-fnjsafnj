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

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

Token get_next_token(const char **input) {
    Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    const char *start = *input;
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        char *ptr = token.value;
        while (**input && isdigit(**input) && (size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        char *ptr = token.value;
        while (**input && (isalnum(**input) || **input == '_') && (size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        char *ptr = token.value;
        while (**input && is_operator_char(**input) && (size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (**input == '"') {
        token.type = TOKEN_STRING;
        char *ptr = token.value;
        (*input)++;
        while (**input && **input != '"' && (size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
            (*input)++;
        }
        *ptr = '\0';
        if (**input == '"') {
            (*input)++;
        }
        return token;
    }
    
    token.type = TOKEN_INVALID;
    token.value[0] = **input;
    token.value[1] = '\0';
    (*input)++;
    return token;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    const char *ptr = input;
    
    while (*ptr && count < max_tokens) {
        Token token = get_next_token(&ptr);
        if (token.type == TOKEN_EOF) {
            break;
        }
        tokens[count] = token;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
    }
    
    return count;
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
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
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("%-12s %-16s %s\n", "Index", "Type", "Value");
    printf("------------ ---------------- --------------------\n");
    
    for (int i = 0; i < token_count && i < MAX_TOKENS; i++) {
        printf("%-12d %-16s %s\n", i, token_type_to_string(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}