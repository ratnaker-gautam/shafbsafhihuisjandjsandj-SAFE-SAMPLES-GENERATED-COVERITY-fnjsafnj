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
    
    if (end > start) {
        strncpy(token->value, start, end - start);
        token->value[end - start] = '\0';
        token->type = TOKEN_NUMBER;
        *input = end;
        return 1;
    }
    
    return 0;
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
    
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    if (end > start) {
        strncpy(token->value, start, end - start);
        token->value[end - start] = '\0';
        token->type = TOKEN_IDENTIFIER;
        *input = end;
        return 1;
    }
    
    return 0;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    
    if (*start && is_operator_char(*start)) {
        if (start[0] == '<' && start[1] == '=') {
            if (2 >= MAX_TOKEN_LEN) return 0;
            strncpy(token->value, start, 2);
            token->value[2] = '\0';
            *input = start + 2;
        } else if (start[0] == '>' && start[1] == '=') {
            if (2 >= MAX_TOKEN_LEN) return 0;
            strncpy(token->value, start, 2);
            token->value[2] = '\0';
            *input = start + 2;
        } else {
            if (1 >= MAX_TOKEN_LEN) return 0;
            token->value[0] = *start;
            token->value[1] = '\0';
            *input = start + 1;
        }
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    return 0;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    
    if (*start == '"') {
        char *end = start + 1;
        while (*end && *end != '"') {
            if (*end == '\\' && *(end + 1)) {
                end += 2;
            } else {
                end++;
            }
        }
        
        if (*end == '"') {
            size_t len = end - start + 1;
            if (len >= MAX_TOKEN_LEN) return 0;
            strncpy(token->value, start, len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            *input = end + 1;
            return 1;
        }
    }
    
    return 0;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        if (parse_number(&input, &tokens[token_count])) {
            token_count++;
        } else if (parse_string(&input, &tokens[token_count])) {
            token_count++;
        } else if (parse_identifier(&input, &tokens[token_count])) {
            token_count++;
        } else if (parse_operator(&input, &tokens[token_count])) {
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression