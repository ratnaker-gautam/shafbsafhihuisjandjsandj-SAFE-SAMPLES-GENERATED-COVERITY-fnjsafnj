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
    TOKEN_ERROR
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
    
    strncpy(token->value, op, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    while (**input && isdigit(**input)) {
        if (len < MAX_TOKEN_LEN - 1) {
            *dest++ = **input;
            len++;
        }
        (*input)++;
    }
    *dest = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    if (**input != '"') return 0;
    (*input)++;
    
    while (**input && **input != '"') {
        if (len < MAX_TOKEN_LEN - 1) {
            *dest++ = **input;
            len++;
        }
        (*input)++;
    }
    
    if (**input == '"') {
        (*input)++;
        *dest = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!isalpha(**input) && **input != '_') return 0;
    
    if (len < MAX_TOKEN_LEN - 1) {
        *dest++ = **input;
        len++;
    }
    (*input)++;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        if (len < MAX_TOKEN_LEN - 1) {
            *dest++ = **input;
            len++;
        }
        (*input)++;
    }
    *dest = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    int token_count = 0;
    
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        Token *current = tokens + token_count;
        
        if (parse_string(&ptr, current) ||
            parse_number(&ptr, current) ||
            parse_operator(&ptr, current) ||
            parse_identifier(&ptr, current)) {
            token_count++;
        } else {
            current->type = TOKEN_ERROR;
            strncpy(current->value, "UNKNOWN", MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
            ptr++;
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
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    } else {
        printf("UNKNOWN");
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
    
    int token_count = tokenize(input, tokens,