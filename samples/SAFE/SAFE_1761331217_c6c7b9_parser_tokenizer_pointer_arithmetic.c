//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    char *end;
    double val = strtod(*input, &end);
    if (end == *input) return 0;
    
    int len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    const char *start = *input;
    if (!isalpha(**input) && **input != '_') return 0;
    
    (*input)++;
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    if (!is_operator_char(**input)) return 0;
    
    const char *start = *input;
    (*input)++;
    while (**input && is_operator_char(**input)) {
        (*input)++;
    }
    
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, struct Token *token) {
    if (**input != '"') return 0;
    
    const char *start = *input;
    (*input)++;
    
    while (**input && **input != '"') {
        if (**input == '\\' && *(*input + 1)) {
            (*input) += 2;
        } else {
            (*input)++;
        }
    }
    
    if (**input != '"') return 0;
    
    int len = *input - start + 1;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        skip_whitespace(&input);
        if (!*input) break;
        
        if (parse_number(&input, &tokens[count]) ||
            parse_string(&input, &tokens[count]) ||
            parse_identifier(&input, &tokens[count]) ||
            parse_operator(&input, &tokens[count])) {
            count++;
        } else {
            break;
        }
    }
    
    tokens[count].type = TOKEN_EOF;
    tokens[count].value[0] = '\0';
    return count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
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
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}