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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, char *output) {
    char *end = output;
    int has_dot = 0;
    
    while (*input && (isdigit(*input) || *input == '.')) {
        if (*input == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        *end++ = *input++;
    }
    *end = '\0';
    return end - output > 0;
}

int parse_identifier(const char *input, char *output) {
    char *end = output;
    
    if (*input && (isalpha(*input) || *input == '_')) {
        *end++ = *input++;
        while (*input && (isalnum(*input) || *input == '_')) {
            *end++ = *input++;
        }
        *end = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, char *output) {
    char *end = output;
    
    if (is_operator_char(*input)) {
        *end++ = *input++;
        if (is_operator_char(*input)) {
            *end++ = *input++;
        }
        *end = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, char *output) {
    char *end = output;
    
    if (*input == '"') {
        *end++ = *input++;
        while (*input && *input != '"') {
            *end++ = *input++;
        }
        if (*input == '"') {
            *end++ = *input++;
            *end = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    Token *token_ptr = tokens;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        if (parse_number(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_NUMBER;
        } else if (parse_identifier(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_IDENTIFIER;
        } else if (parse_operator(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_OPERATOR;
        } else if (parse_string(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_STRING;
        } else {
            break;
        }
        
        ptr += strlen(token_ptr->value);
        token_ptr++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        token_ptr->type = TOKEN_EOF;
        strcpy(token_ptr->value, "EOF");
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
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %s -> %s\n", i + 1, token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}