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
    char op[3] = {0};
    
    op[0] = **input;
    (*input)++;
    
    if (**input && is_operator_char(**input)) {
        if ((*start == '<' && **input == '=') ||
            (*start == '>' && **input == '=') ||
            (*start == '!' && **input == '=') ||
            (*start == '=' && **input == '=') ||
            (*start == '&' && **input == '&') ||
            (*start == '|' && **input == '|')) {
            op[1] = **input;
            (*input)++;
        }
    }
    
    strncpy(token->value, op, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *end = token->value;
    int max_len = MAX_TOKEN_LEN - 1;
    
    while (**input && isdigit(**input) && max_len > 0) {
        *end++ = **input;
        (*input)++;
        max_len--;
    }
    *end = '\0';
    
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    
    (*input)++;
    char *end = token->value;
    int max_len = MAX_TOKEN_LEN - 1;
    
    while (**input && **input != '"' && max_len > 0) {
        *end++ = **input;
        (*input)++;
        max_len--;
    }
    
    if (**input != '"') {
        return 0;
    }
    (*input)++;
    
    *end = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *end = token->value;
    int max_len = MAX_TOKEN_LEN - 1;
    
    if (!isalpha(**input) && **input != '_') return 0;
    
    *end++ = **input;
    (*input)++;
    max_len--;
    
    while (**input && (isalnum(**input) || **input == '_') && max_len > 0) {
        *end++ = **input;
        (*input)++;
        max_len--;
    }
    *end = '\0';
    
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
    
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
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    const char *cursor = input;
    
    while (*cursor && token_count < MAX_TOKENS) {
        if (!get_next_token(&cursor, &tokens[token_count])) {
            break;
        }
        if (tokens[token_count].type == TOKEN_EOF) {
            break;
        }
        token_count++;
    }
    
    printf("