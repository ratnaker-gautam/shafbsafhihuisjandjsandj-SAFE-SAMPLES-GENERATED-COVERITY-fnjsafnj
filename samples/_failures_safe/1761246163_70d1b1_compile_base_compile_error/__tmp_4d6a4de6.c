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

int parse_number(const char *input, char *output) {
    char *end = output;
    int has_dot = 0;
    
    while (*input && (isdigit(*input) || *input == '.')) {
        if (*input == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) break;
        *end++ = *input++;
    }
    *end = '\0';
    return end - output;
}

int parse_string(const char *input, char *output) {
    if (*input != '"') return 0;
    
    char *end = output;
    input++;
    
    while (*input && *input != '"') {
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) break;
        *end++ = *input++;
    }
    
    if (*input == '"') {
        *end = '\0';
        return (end - output) + 2;
    }
    
    return 0;
}

int parse_identifier(const char *input, char *output) {
    if (!isalpha(*input) && *input != '_') return 0;
    
    char *end = output;
    *end++ = *input++;
    
    while (*input && (isalnum(*input) || *input == '_')) {
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) break;
        *end++ = *input++;
    }
    *end = '\0';
    return end - output;
}

int parse_operator(const char *input, char *output) {
    if (!is_operator_char(*input)) return 0;
    
    char *end = output;
    *end++ = *input++;
    
    if (is_operator_char(*input) && 
        ((*(end-1) == '&' && *input == '&') ||
         (*(end-1) == '|' && *input == '|') ||
         (*(end-1) == '=' && *input == '=') ||
         (*(end-1) == '!' && *input == '=') ||
         (*(end-1) == '<' && *input == '=') ||
         (*(end-1) == '>' && *input == '='))) {
        if ((size_t)(end - output) < MAX_TOKEN_LEN - 1) {
            *end++ = *input++;
        }
    }
    
    *end = '\0';
    return end - output;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    Token *token_ptr = tokens;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        int len = 0;
        TokenType type = TOKEN_UNKNOWN;
        
        if ((len = parse_string(ptr, token_ptr->value)) > 0) {
            type = TOKEN_STRING;
        } else if ((len = parse_number(ptr, token_ptr->value)) > 0) {
            type = TOKEN_NUMBER;
        } else if ((len = parse_identifier(ptr, token_ptr->value)) > 0) {
            type = TOKEN_IDENTIFIER;
        } else if ((len = parse_operator(ptr, token_ptr->value)) > 0) {
            type = TOKEN_OPERATOR;
        }
        
        if (len > 0) {
            token_ptr->type = type;
            token_ptr++;
            token_count++;
            ptr += len;
        } else {
            token_ptr->type = TOKEN_UNKNOWN;
            if (MAX_TOKEN_LEN > 1) {
                token_ptr->value[0] = *ptr;
                token_ptr->value[1] = '\0';
            }
            token_ptr++;
            token_count++;
            ptr++;
        }
    }
    
    if (token_count < max_tokens) {
        token_ptr->type = TOKEN_EOF;
        token_ptr->value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
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
    
    size