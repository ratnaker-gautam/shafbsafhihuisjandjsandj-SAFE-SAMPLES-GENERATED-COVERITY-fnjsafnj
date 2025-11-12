//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
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

int parse_identifier(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
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

int parse_operator(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
    if (!is_operator_char(*end)) {
        return 0;
    }
    
    end++;
    if (is_operator_char(*end)) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = end;
    return 1;
}

int parse_string(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
    if (*end != '"') {
        return 0;
    }
    
    end++;
    while (*end && *end != '"') {
        if (*end == '\\' && *(end + 1)) {
            end += 2;
        } else {
            end++;
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

int tokenize(char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        struct Token *current_token = tokens + token_count;
        
        if (parse_number(&input, current_token) ||
            parse_identifier(&input, current_token) ||
            parse_operator(&input, current_token) ||
            parse_string(&input, current_token)) {
            token_count++;
        } else {
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input_buffer[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input_buffer, tokens, MAX_TOKENS);
    
    if (token_count