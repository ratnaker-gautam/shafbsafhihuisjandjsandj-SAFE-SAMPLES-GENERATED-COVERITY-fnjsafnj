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
    char *ptr = *input;
    
    if (!isdigit(*ptr)) {
        return 0;
    }
    
    while (*ptr && isdigit(*ptr)) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_identifier(char **input, struct Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!isalpha(*ptr) && *ptr != '_') {
        return 0;
    }
    
    ptr++;
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, struct Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!is_operator_char(*ptr)) {
        return 0;
    }
    
    ptr++;
    if (is_operator_char(*ptr)) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int parse_string(char **input, struct Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (*ptr != '"') {
        return 0;
    }
    
    ptr++;
    while (*ptr && *ptr != '"') {
        if (*ptr == '\\' && *(ptr + 1)) {
            ptr += 2;
        } else {
            ptr++;
        }
    }
    
    if (*ptr != '"') {
        return 0;
    }
    
    ptr++;
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr;
    return 1;
}

int tokenize(char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        struct Token token;
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
            tokens[token_count] = token;
            token_count++;
        } else {
            input++;
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
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {