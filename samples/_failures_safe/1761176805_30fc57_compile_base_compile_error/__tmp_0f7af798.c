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
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (!isalpha(*end) && *end != '_') {
        return 0;
    }
    
    end++;
    while (*end && (isalnum(*end) || *end == '_')) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN) {
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
    
    if (!is_operator_char(*end)) {
        return 0;
    }
    
    end++;
    if (end - start >= MAX_TOKEN_LEN) {
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
    
    if (!*end) {
        return 0;
    }
    
    end++;
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_STRING;
    *input = end;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        if (parse_number(&input, &tokens[count])) {
            count++;
            continue;
        }
        
        if (parse_identifier(&input, &tokens[count])) {
            count++;
            continue;
        }
        
        if (parse_operator(&input, &tokens[count])) {
            count++;
            continue;
        }
        
        if (parse_string(&input, &tokens[count])) {
            count++;
            continue;
        }
        
        break;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "");
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
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count <= 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for