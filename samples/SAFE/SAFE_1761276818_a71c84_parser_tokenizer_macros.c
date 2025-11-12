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
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && strchr(DELIMITERS, **input)) {
        (*input)++;
    }
}

int parse_token(const char **input, Token *token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        return 1;
    }
    
    char current = **input;
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while ((**input && (isalnum(**input) || **input == '_')) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        int i = 0;
        while (**input && isdigit(**input) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (is_operator(current)) {
        int i = 0;
        while (**input && is_operator(**input) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
    } else if (current == '"') {
        (*input)++;
        int i = 0;
        while (**input && **input != '"' && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = *(*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    } else {
        return 0;
    }
    
    return 1;
}

int tokenize(const char *input, Token tokens[], int max_tokens) {
    int count = 0;
    const char *ptr = input;
    
    while (*ptr && count < max_tokens) {
        if (!parse_token(&ptr, &tokens[count])) {
            return -1;
        }
        if (tokens[count].type == TOKEN_EOF) {
            break;
        }
        count++;
    }
    
    return count;
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

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Error during tokenization\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=%s, Value='%s'\n", 
               i + 1, token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}