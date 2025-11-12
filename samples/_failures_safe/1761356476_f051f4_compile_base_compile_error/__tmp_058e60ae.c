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

int parse_operator(const char *input, char *output) {
    if (!input || !output) return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    if (is_operator_char(*input)) {
        *out_ptr++ = *input++;
        if ((*start == '<' && *input == '=') ||
            (*start == '>' && *input == '=') ||
            (*start == '!' && *input == '=') ||
            (*start == '&' && *input == '&') ||
            (*start == '|' && *input == '|')) {
            *out_ptr++ = *input++;
        }
        *out_ptr = '\0';
        return (int)(input - start);
    }
    return 0;
}

int parse_number(const char *input, char *output) {
    if (!input || !output) return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    while (*input && isdigit(*input)) {
        if ((out_ptr - output) >= (MAX_TOKEN_LEN - 1)) break;
        *out_ptr++ = *input++;
    }
    *out_ptr = '\0';
    
    return (out_ptr > output) ? (int)(input - start) : 0;
}

int parse_string(const char *input, char *output) {
    if (!input || !output || *input != '"') return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    input++;
    while (*input && *input != '"') {
        if ((out_ptr - output) >= (MAX_TOKEN_LEN - 1)) break;
        *out_ptr++ = *input++;
    }
    
    if (*input == '"') {
        input++;
        *out_ptr = '\0';
        return (int)(input - start);
    }
    return 0;
}

int parse_identifier(const char *input, char *output) {
    if (!input || !output || !isalpha(*input)) return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    while (*input && (isalnum(*input) || *input == '_')) {
        if ((out_ptr - output) >= (MAX_TOKEN_LEN - 1)) break;
        *out_ptr++ = *input++;
    }
    *out_ptr = '\0';
    
    return (int)(input - start);
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    Token *token_ptr = tokens;
    const char *ptr = input;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        int consumed = 0;
        
        if ((consumed = parse_operator(ptr, token_ptr->value)) > 0) {
            token_ptr->type = TOKEN_OPERATOR;
        } else if ((consumed = parse_number(ptr, token_ptr->value)) > 0) {
            token_ptr->type = TOKEN_NUMBER;
        } else if ((consumed = parse_string(ptr, token_ptr->value)) > 0) {
            token_ptr->type = TOKEN_STRING;
        } else if ((consumed = parse_identifier(ptr, token_ptr->value)) > 0) {
            token_ptr->type = TOKEN_IDENTIFIER;
        } else {
            token_ptr->type = TOKEN_INVALID;
            token_ptr->value[0] = *ptr;
            token_ptr->value[1] = '\0';
            consumed = 1;
        }
        
        ptr += consumed;
        token_ptr++;
        token_count++;
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
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_INVALID) {
        printf("%s", names[type]);
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
    
    size_t len