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
    
    if (dest - token->value > 0) {
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int has_digit = 0;
    
    while (isdigit((unsigned char)**input) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
        has_digit = 1;
    }
    
    if (**input == '.' && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
        while (isdigit((unsigned char)**input) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
            *dest++ = *(*input)++;
            has_digit = 1;
        }
    }
    
    if (has_digit) {
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
        (*input)++;
        *dest = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    
    if (!isalpha((unsigned char)**input) && **input != '_') return 0;
    
    *dest++ = *(*input)++;
    while ((isalnum((unsigned char)**input) || **input == '_') && 
           (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    *dest = '\0';
    
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

void skip_whitespace(const char **input) {
    while (isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    size_t token_count = 0;
    
    while (*input != '\0' && token_count < max_tokens) {
        skip_whitespace(&input);
        if (*input == '\0') break;
        
        Token *current = tokens + token_count;
        
        if (parse_string(&input, current) ||
            parse_number(&input, current) ||
            parse_operator(&input, current) ||
            parse_identifier(&input, current)) {
            token_count++;
        } else {
            current->type = TOKEN_INVALID;
            char *dest = current->value;
            *dest++ = *input++;
            *dest = '\0';
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return (int)token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_INVALID) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen