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
    TOKEN_ERROR
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
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    *end = '\0';
    return end - output;
}

int parse_identifier(const char *input, char *output) {
    char *end = output;
    
    if (!isalpha(*input) && *input != '_') return 0;
    
    *end++ = *input++;
    while (*input && (isalnum(*input) || *input == '_')) {
        *end++ = *input++;
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    *end = '\0';
    return end - output;
}

int parse_operator(const char *input, char *output) {
    char *end = output;
    
    if (!is_operator_char(*input)) return 0;
    
    *end++ = *input++;
    if (*input && is_operator_char(*input) && *input == '=') {
        *end++ = *input++;
    }
    *end = '\0';
    return end - output;
}

int parse_string(const char *input, char *output) {
    char *end = output;
    
    if (*input != '"') return 0;
    
    *end++ = *input++;
    while (*input && *input != '"') {
        if (*input == '\\' && *(input + 1)) {
            *end++ = *input++;
            *end++ = *input++;
        } else {
            *end++ = *input++;
        }
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    if (*input != '"') return 0;
    *end++ = *input++;
    *end = '\0';
    return end - output;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        int len;
        Token *current = tokens + token_count;
        
        if ((len = parse_number(ptr, current->value))) {
            current->type = TOKEN_NUMBER;
            ptr += len;
        } else if ((len = parse_identifier(ptr, current->value))) {
            current->type = TOKEN_IDENTIFIER;
            ptr += len;
        } else if ((len = parse_operator(ptr, current->value))) {
            current->type = TOKEN_OPERATOR;
            ptr += len;
        } else if ((len = parse_string(ptr, current->value))) {
            current->type = TOKEN_STRING;
            ptr += len;
        } else {
            current->type = TOKEN_ERROR;
            current->value[0] = *ptr;
            current->value[1] = '\0';
            ptr++;
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
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
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
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: Type=", i);
        print_token_type