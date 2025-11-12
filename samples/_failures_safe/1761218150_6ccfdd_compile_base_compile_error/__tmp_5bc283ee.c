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
        op[1] = **input;
        (*input)++;
    }
    
    if (strlen(op) > MAX_TOKEN_LEN - 1) {
        return 0;
    }
    
    strcpy(token->value, op);
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    while (**input && isdigit(**input)) {
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *dest++ = **input;
        (*input)++;
        len++;
    }
    *dest = '\0';
    
    if (len > 0) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        *dest++ = **input;
        (*input)++;
        len++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) {
                return 0;
            }
            *dest++ = **input;
            (*input)++;
            len++;
        }
        *dest = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    if (**input == '"') {
        (*input)++;
        
        while (**input && **input != '"') {
            if (len >= MAX_TOKEN_LEN - 1) {
                return 0;
            }
            *dest++ = **input;
            (*input)++;
            len++;
        }
        
        if (**input == '"') {
            (*input)++;
            *dest = '\0';
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
        while (*ptr && isspace(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) {
            break;
        }
        
        Token *current_token = tokens + token_count;
        
        if (parse_string(&ptr, current_token) ||
            parse_number(&ptr, current_token) ||
            parse_identifier(&ptr, current_token) ||
            parse_operator(&ptr, current_token)) {
            token_count++;
        } else {
            current_token->type = TOKEN_INVALID;
            current_token->value[0] = *ptr;
            current_token->value[1] = '\0';
            ptr++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
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
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
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
    
    int token_count = tokenize(input, tokens, MAX_TOK