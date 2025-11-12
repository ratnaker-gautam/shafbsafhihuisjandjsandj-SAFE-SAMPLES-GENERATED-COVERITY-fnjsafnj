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
    TOKEN_UNKNOWN
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
    
    if (start == *input) return 0;
    
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *end = token->value;
    int max_len = MAX_TOKEN_LEN - 1;
    
    if (**input && (isalpha(**input) || **input == '_')) {
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
    return 0;
}

int parse_string(const char **input, Token *token) {
    const char *start = *input;
    char *end = token->value;
    int max_len = MAX_TOKEN_LEN - 1;
    
    if (**input == '"') {
        (*input)++;
        
        while (**input && **input != '"' && max_len > 0) {
            *end++ = **input;
            (*input)++;
            max_len--;
        }
        
        if (**input == '"') {
            (*input)++;
            *end = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    *input = start;
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    const char *ptr = input;
    
    while (*ptr && count < max_tokens) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        if (parse_string(&ptr, &tokens[count]) ||
            parse_number(&ptr, &tokens[count]) ||
            parse_identifier(&ptr, &tokens[count]) ||
            parse_operator(&ptr, &tokens[count])) {
            count++;
        } else {
            tokens[count].value[0] = *ptr;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_UNKNOWN;
            ptr++;
            count++;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    
    return count;
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
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
    if (len