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
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *start, const char *end, char *output) {
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
    
    if (p - start >= MAX_TOKEN_LEN) return 0;
    memcpy(output, start, p - start);
    output[p - start] = '\0';
    return p - start;
}

int parse_string(const char *start, const char *end, char *output) {
    if (*start != '"') return 0;
    
    const char *p = start + 1;
    int len = 0;
    
    while (p < end && *p != '"' && len < MAX_TOKEN_LEN - 1) {
        if (*p == '\\' && p + 1 < end) {
            p++;
        }
        output[len++] = *p++;
    }
    
    if (p >= end || *p != '"') return 0;
    output[len] = '\0';
    return p - start + 1;
}

int parse_identifier(const char *start, const char *end, char *output) {
    if (!isalpha(*start) && *start != '_') return 0;
    
    const char *p = start + 1;
    int len = 1;
    output[0] = *start;
    
    while (p < end && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
        output[len++] = *p++;
    }
    
    output[len] = '\0';
    return len;
}

int parse_operator(const char *start, const char *end, char *output) {
    if (!is_operator_char(*start)) return 0;
    
    int len = 1;
    output[0] = *start;
    
    if (start + 1 < end && is_operator_char(*(start + 1))) {
        output[1] = *(start + 1);
        len = 2;
    }
    
    output[len] = '\0';
    return len;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        int consumed = 0;
        Token *current = tokens + token_count;
        
        if ((consumed = parse_string(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_STRING;
            token_count++;
        } else if ((consumed = parse_number(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_NUMBER;
            token_count++;
        } else if ((consumed = parse_identifier(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if ((consumed = parse_operator(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_OPERATOR;
            token_count++;
        } else {
            current->type = TOKEN_ERROR;
            current->value[0] = *p;
            current->value[1] = '\0';
            token_count++;
            consumed = 1;
        }
        
        p += consumed;
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
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_ERROR) {
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
    
    if (len == 0) {
        fprintf(st