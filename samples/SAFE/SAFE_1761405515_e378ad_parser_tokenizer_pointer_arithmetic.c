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
    TOKEN_ERROR
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

Token get_next_token(char **input) {
    Token token = {TOKEN_ERROR, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char *start = *input;
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        char *ptr = token.value;
        while (**input && isdigit(**input)) {
            if ((size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        char *ptr = token.value;
        while (**input && (isalnum(**input) || **input == '_')) {
            if ((size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        char *ptr = token.value;
        while (**input && is_operator_char(**input)) {
            if ((size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (**input == '"') {
        token.type = TOKEN_STRING;
        char *ptr = token.value;
        (*input)++;
        while (**input && **input != '"') {
            if ((size_t)(ptr - token.value) < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        if (**input == '"') {
            (*input)++;
        }
        return token;
    }
    
    (*input)++;
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    while (*input && count < max_tokens) {
        Token token = get_next_token(&input);
        if (token.type == TOKEN_EOF) {
            break;
        }
        if (token.type != TOKEN_ERROR) {
            tokens[count] = token;
            count++;
        } else {
            break;
        }
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
        default: return "ERROR";
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
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=%s, Value='%s'\n", 
               i + 1, 
               token_type_to_string(tokens[i].type),
               tokens[i].value);
    }
    
    return 0;
}