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

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    *dest++ = *p++;
    len++;
    
    while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!is_operator_char(*p)) return 0;
    
    *dest++ = *p++;
    len++;
    
    if (*p && is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p != '"') return 0;
    
    p++;
    while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    
    if (*p != '"') {
        token->type = TOKEN_ERROR;
        return 0;
    }
    
    p++;
    *dest = '\0';
    *input = p;
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *current = tokens;
    int count = 0;
    
    while (*p && count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        if (parse_number(&p, current) ||
            parse_identifier(&p, current) ||
            parse_operator(&p, current) ||
            parse_string(&p, current)) {
            count++;
            current++;
        } else {
            current->type = TOKEN_ERROR;
            strncpy(current->value, "Unknown", MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            count++;
            current++;
            p++;
        }
    }
    
    if (count < max_tokens) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
        count++;
    }
    
    return count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s", i, type_names[tokens[i].type]);
        if (tokens[i].value[0] != '\0') {
            printf(" ('%s')", tokens[i].value);
        }
        printf("\n");
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Warning: Input too large