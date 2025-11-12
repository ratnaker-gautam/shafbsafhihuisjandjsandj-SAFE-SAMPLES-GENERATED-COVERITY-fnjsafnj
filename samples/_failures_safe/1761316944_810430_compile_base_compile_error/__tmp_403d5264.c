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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    while (*p && isdigit(*p)) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    *end = '\0';
    token->type = TOKEN_NUMBER;
    *input = p;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (!isalpha(*p) && *p != '_') {
        return 0;
    }
    
    while (*p && (isalnum(*p) || *p == '_')) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    *end = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = p;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (!is_operator_char(*p)) {
        return 0;
    }
    
    *end++ = *p++;
    if (is_operator_char(*p) && (*(p-1) == '<' || *(p-1) == '>' || *(p-1) == '!' || *(p-1) == '=')) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    *end = '\0';
    token->type = TOKEN_OPERATOR;
    *input = p;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (*p != '"') {
        return 0;
    }
    p++;
    
    while (*p && *p != '"') {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    if (*p != '"') {
        return 0;
    }
    p++;
    
    *end = '\0';
    token->type = TOKEN_STRING;
    *input = p;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    const char *p = *input;
    
    while (*p && isspace(*p)) {
        p++;
    }
    
    if (!*p) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        *input = p;
        return 1;
    }
    
    if (parse_number(&p, token)) return 1;
    if (parse_string(&p, token)) return 1;
    if (parse_identifier(&p, token)) return 1;
    if (parse_operator(&p, token)) return 1;
    
    token->type = TOKEN_ERROR;
    token->value[0] = *p;
    token->value[1] = '\0';
    p++;
    *input = p;
    return 1;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    const char *cursor = input;
    
    while (token_count < MAX_TOKENS) {
        if (!get_next_token(&cursor, &tokens[token_count])) {
            fprintf(stderr, "Tokenization failed\n");
            return 1;
        }
        
        if (tokens[token_count].type == TOKEN_EOF) {
            break;
        }
        
        if (tokens[token_count].type == TOKEN_ERROR) {
            fprintf(stderr, "Invalid character: %s\n", tokens[token_count].value);
            return 1;
        }
        
        token_count++;
    }
    
    if (token_count >=