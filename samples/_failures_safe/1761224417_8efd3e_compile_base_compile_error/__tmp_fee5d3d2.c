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
    TOKEN_INVALID
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

int parse_string(const char *input, char *output) {
    if (*input != '"') return 0;
    
    char *end = output;
    input++;
    
    while (*input && *input != '"') {
        if (*input == '\\' && *(input + 1)) {
            *end++ = *(input + 1);
            input += 2;
        } else {
            *end++ = *input++;
        }
    }
    
    if (*input != '"') return 0;
    *end = '\0';
    return 1;
}

int parse_identifier(const char *input, char *output) {
    if (!isalpha(*input) && *input != '_') return 0;
    
    char *end = output;
    *end++ = *input++;
    
    while (*input && (isalnum(*input) || *input == '_')) {
        *end++ = *input++;
    }
    *end = '\0';
    return 1;
}

int parse_operator(const char *input, char *output) {
    if (!is_operator_char(*input)) return 0;
    
    char *end = output;
    *end++ = *input++;
    
    if (is_operator_char(*input) && (*(end - 1) == '=' || *(end - 1) == '<' || *(end - 1) == '>' || *(end - 1) == '!')) {
        *end++ = *input++;
    }
    
    *end = '\0';
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        Token *token = tokens + token_count;
        
        if (parse_number(ptr, token->value)) {
            token->type = TOKEN_NUMBER;
        } else if (parse_string(ptr, token->value)) {
            token->type = TOKEN_STRING;
            ptr += strlen(token->value) + 2;
            if (*(ptr - 1) != '"') return -1;
        } else if (parse_identifier(ptr, token->value)) {
            token->type = TOKEN_IDENTIFIER;
        } else if (parse_operator(ptr, token->value)) {
            token->type = TOKEN_OPERATOR;
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = *ptr;
            token->value[1] = '\0';
            ptr++;
            token_count++;
            continue;
        }
        
        if (token->type != TOKEN_STRING) {
            ptr += strlen(token->value);
        }
        token_count++;
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
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    printf("%s", names[type]);
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
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization error\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0