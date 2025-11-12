//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    
    while (is_operator_char(**input) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    *dest = '\0';
    
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int has_dot = 0;
    
    while ((isdigit(**input) || **input == '.') && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        if (**input == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        *dest++ = *(*input)++;
    }
    
    if (dest > token->value) {
        *dest = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    
    const char *start = *input;
    char *dest = token->value;
    (*input)++;
    
    while (**input != '"' && **input != '\0' && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    
    if (**input == '"') {
        *dest = '\0';
        (*input)++;
        token->type = TOKEN_STRING;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    if (!isalpha(**input) && **input != '_') return 0;
    
    const char *start = *input;
    char *dest = token->value;
    
    while ((isalnum(**input) || **input == '_') && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    *dest = '\0';
    
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    while (isspace(**input)) (*input)++;
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_string(input, token)) return 1;
    if (parse_number(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 1;
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    const char *current = input;
    Token *token_ptr = tokens;
    Token *end = tokens + max_tokens;
    
    while (*current != '\0' && token_ptr < end) {
        if (!get_next_token(&current, token_ptr)) {
            break;
        }
        if (token_ptr->type == TOKEN_EOF) {
            break;
        }
        token_ptr++;
    }
    
    if (token_ptr < end) {
        token_ptr->type = TOKEN_EOF;
        token_ptr->value[0] = '\0';
    }
    
    return (int)(token_ptr - tokens);
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    const Token *current = tokens;
    const Token *end = tokens + count;
    
    while (current < end && current->type != TOKEN_EOF) {
        printf("%-12s: '%s'\n", type_names[current->type], current->value);
        current++;
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
    if (len >