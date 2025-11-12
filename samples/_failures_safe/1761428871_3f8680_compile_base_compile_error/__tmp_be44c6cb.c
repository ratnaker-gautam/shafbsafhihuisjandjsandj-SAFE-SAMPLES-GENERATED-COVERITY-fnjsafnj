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
    
    if (end - start > 0) {
        size_t len = end - start;
        if (len >= MAX_TOKEN_LEN) {
            return 0;
        }
        memcpy(token->value, start, len);
        token->value[len] = '\0';
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
        
        size_t len = end - start;
        if (len >= MAX_TOKEN_LEN) {
            return 0;
        }
        memcpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        *input = end;
        return 1;
    }
    return 0;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    
    if (*start && is_operator_char(*start)) {
        size_t len = 1;
        if (*(start + 1) && is_operator_char(*(start + 1))) {
            len = 2;
        }
        
        if (len >= MAX_TOKEN_LEN) {
            return 0;
        }
        memcpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        *input += len;
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
            if (len >= MAX_TOKEN_LEN) {
                return 0;
            }
            memcpy(token->value, start, len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            *input = end + 1;
            return 1;
        }
    }
    return 0;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_number(&input, &token)) {
            parsed = 1;
        } else if (parse_string(&input, &token)) {
            parsed = 1;
        } else if (parse_identifier(&input, &token)) {
            parsed = 1;
        } else if (parse_operator(&input, &token)) {
            parsed = 1;
        }
        
        if (parsed) {
            tokens[count] = token;
            count++;
        } else {
            break;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    
    return count;
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
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {