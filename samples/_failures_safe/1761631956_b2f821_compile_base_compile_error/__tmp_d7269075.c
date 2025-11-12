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
    
    if (strlen(op) > 0) {
        strncpy(token->value, op, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *end = NULL;
    
    strtod(*input, &end);
    if (end > *input) {
        size_t len = end - *input;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, *input, len);
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            *input = end;
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int parse_string(const char **input, Token *token) {
    const char *start = *input;
    
    if (**input == '"') {
        (*input)++;
        const char *str_start = *input;
        
        while (**input && **input != '"') {
            (*input)++;
        }
        
        if (**input == '"') {
            size_t len = *input - str_start;
            if (len < MAX_TOKEN_LEN) {
                strncpy(token->value, str_start, len);
                token->value[len] = '\0';
                token->type = TOKEN_STRING;
                (*input)++;
                return 1;
            }
        }
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        const char *id_start = *input;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            (*input)++;
        }
        
        size_t len = *input - id_start;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, id_start, len);
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        while (isspace(*input)) {
            input++;
        }
        
        if (!*input) {
            break;
        }
        
        Token token = {TOKEN_INVALID, ""};
        
        if (parse_string(&input, &token) ||
            parse_number(&input, &token) ||
            parse_operator(&input, &token) ||
            parse_identifier(&input, &token)) {
            
            if (count < max_tokens) {
                tokens[count] = token;
                count++;
            }
        } else {
            token.value[0] = *input;
            token.value[1] = '\0';
            token.type = TOKEN_INVALID;
            tokens[count] = token;
            count++;
            input++;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    
    return count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
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