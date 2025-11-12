//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    while (*end && isdigit(*end)) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (*end && (isalpha(*end) || *end == '_')) {
        end++;
        while (*end && (isalnum(*end) || *end == '_')) {
            end++;
        }
    }
    
    if (end == start || end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = end;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (*end && is_operator_char(*end)) {
        end++;
        if (*end && is_operator_char(*end)) {
            end++;
        }
    }
    
    if (end == start || end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = end;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (*end != '"') {
        return 0;
    }
    
    end++;
    while (*end && *end != '"') {
        end++;
        if (end - start >= MAX_TOKEN_LEN) {
            return 0;
        }
    }
    
    if (*end != '"') {
        return 0;
    }
    
    end++;
    strncpy(token->value, start + 1, end - start - 2);
    token->value[end - start - 2] = '\0';
    token->type = TOKEN_STRING;
    *input = end;
    return 1;
}

int get_next_token(char **input, Token *token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, token)) return 1;
    if (parse_string(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    
    return 0;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input_buffer[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *input_ptr = input_buffer;
    
    while (token_count < MAX_TOKENS) {
        if (!get_next_token(&input_ptr, &tokens[token_count])) {
            fprintf(stderr, "Invalid token at position: %ld\n", input_ptr - input_buffer);
            return 1;
        }
        
        if (tokens[token_count].type == TOKEN_EOF) {
            break;
        }
        
        token_count++;
    }
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf