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

int parse_number(const char *start, const char **end) {
    const char *p = start;
    if (*p == '-' || *p == '+') p++;
    if (!isdigit(*p)) return 0;
    
    while (isdigit(*p)) p++;
    if (*p == '.') {
        p++;
        while (isdigit(*p)) p++;
    }
    
    *end = p;
    return 1;
}

int parse_string(const char *start, const char **end) {
    if (*start != '"') return 0;
    
    const char *p = start + 1;
    while (*p && *p != '"') {
        if (*p == '\\' && *(p + 1)) p += 2;
        else p++;
    }
    
    if (*p != '"') return 0;
    *end = p + 1;
    return 1;
}

int parse_identifier(const char *start, const char **end) {
    if (!isalpha(*start) && *start != '_') return 0;
    
    const char *p = start + 1;
    while (isalnum(*p) || *p == '_') p++;
    
    *end = p;
    return 1;
}

int parse_operator(const char *start, const char **end) {
    if (!is_operator_char(*start)) return 0;
    
    const char *p = start + 1;
    if ((*start == '&' && *p == '&') || (*start == '|' && *p == '|') ||
        (*start == '=' && *p == '=') || (*start == '!' && *p == '=') ||
        (*start == '<' && *p == '=') || (*start == '>' && *p == '=')) {
        p++;
    }
    
    *end = p;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        const char *token_start = p;
        const char *token_end = NULL;
        
        if (parse_number(p, &token_end)) {
            tokens[token_count].type = TOKEN_NUMBER;
        } else if (parse_string(p, &token_end)) {
            tokens[token_count].type = TOKEN_STRING;
        } else if (parse_identifier(p, &token_end)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
        } else if (parse_operator(p, &token_end)) {
            tokens[token_count].type = TOKEN_OPERATOR;
        } else {
            tokens[token_count].type = TOKEN_INVALID;
            token_end = p + 1;
        }
        
        size_t len = token_end - token_start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(tokens[token_count].value, token_start, len);
        tokens[token_count].value[len] = '\0';
        
        token_count++;
        p = token_end;
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
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token