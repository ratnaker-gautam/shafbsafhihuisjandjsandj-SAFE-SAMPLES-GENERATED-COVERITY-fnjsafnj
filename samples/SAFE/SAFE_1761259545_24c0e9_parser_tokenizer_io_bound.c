//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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

int parse_number(const char **input, Token *token) {
    int i = 0;
    while (**input && isdigit(**input) && i < MAX_TOKEN_LEN - 1) {
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return i > 0;
}

int parse_identifier(const char **input, Token *token) {
    int i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && (isalnum(**input) || **input == '_') && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, Token *token) {
    int i = 0;
    if (**input && is_operator_char(**input)) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && is_operator_char(**input) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, Token *token) {
    int i = 0;
    if (**input == '"') {
        (*input)++;
        while (**input && **input != '"' && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        if (parse_number(&ptr, &tokens[token_count]) ||
            parse_identifier(&ptr, &tokens[token_count]) ||
            parse_operator(&ptr, &tokens[token_count]) ||
            parse_string(&ptr, &tokens[token_count])) {
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[MAX_INPUT_LINE];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=%s, Value='%s'\n", 
               i, token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}