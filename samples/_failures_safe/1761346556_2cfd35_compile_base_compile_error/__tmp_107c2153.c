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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *start, const char *end, Token *token) {
    const char *p = start;
    int has_dot = 0;
    
    while (p < end) {
        if (*p == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else if (!isdigit(*p)) {
            break;
        }
        p++;
    }
    
    if (p == start) return 0;
    
    size_t len = p - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *start, const char *end, Token *token) {
    if (*start != '"') return 0;
    
    const char *p = start + 1;
    while (p < end && *p != '"') {
        if (*p == '\\' && p + 1 < end) p++;
        p++;
    }
    
    if (p >= end || *p != '"') return 0;
    
    size_t len = p - start + 1;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char *start, const char *end, Token *token) {
    if (!is_operator_char(*start)) return 0;
    
    const char *p = start + 1;
    if (p < end && is_operator_char(*p)) p++;
    
    size_t len = p - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_identifier(const char *start, const char *end, Token *token) {
    if (!isalpha(*start) && *start != '_') return 0;
    
    const char *p = start + 1;
    while (p < end && (isalnum(*p) || *p == '_')) p++;
    
    size_t len = p - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        const char *start = p;
        const char *end = p;
        while (*end && !isspace(*end)) end++;
        
        Token token = {TOKEN_INVALID, ""};
        int parsed = 0;
        
        if (!parsed) parsed = parse_string(start, end, &token);
        if (!parsed) parsed = parse_number(start, end, &token);
        if (!parsed) parsed = parse_operator(start, end, &token);
        if (!parsed) parsed = parse_identifier(start, end, &token);
        
        if (parsed) {
            tokens[token_count++] = token;
            p = start + strlen(token.value);
        } else {
            p = end;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
        if (tokens[i].type == TOKEN_EOF) break;
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
    if (len > 0 &&