//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    char *end;
    long num = strtol(start, &end, 10);
    
    if (end > start && num >= 0) {
        size_t len = end - start;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, start, len);
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            *input = end;
            return 1;
        }
    }
    return 0;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    
    const char *start = *input + 1;
    const char *ptr = start;
    
    while (*ptr && *ptr != '"') {
        ptr++;
    }
    
    if (*ptr != '"') return 0;
    
    size_t len = ptr - start;
    if (len < MAX_TOKEN_LEN - 1) {
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        *input = ptr + 1;
        return 1;
    }
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    
    if (!isalpha(*start) && *start != '_') return 0;
    
    const char *ptr = start + 1;
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len < MAX_TOKEN_LEN) {
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        *input = ptr;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        Token *current = tokens + token_count;
        
        if (parse_number(&ptr, current) ||
            parse_string(&ptr, current) ||
            parse_identifier(&ptr, current) ||
            parse_operator(&ptr, current)) {
            token_count++;
        } else {
            current->type = TOKEN_INVALID;
            strncpy(current->value, ptr, 1);
            current->value[1] = '\0';
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
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len -