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

TokenType classify_token(const char *str) {
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_INVALID;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        Token *current = tokens + token_count;
        const char *start = p;
        
        current->type = classify_token(p);
        
        if (current->type == TOKEN_NUMBER) {
            while (isdigit(*p)) p++;
        } else if (current->type == TOKEN_IDENTIFIER) {
            while (isalnum(*p) || *p == '_') p++;
        } else if (current->type == TOKEN_OPERATOR) {
            p++;
            if (is_operator_char(*p) && (*(p-1) == '=' || *(p-1) == '!' || *(p-1) == '<' || *(p-1) == '>')) {
                p++;
            }
        } else if (current->type == TOKEN_STRING) {
            p++;
            while (*p && *p != '"') p++;
            if (*p == '"') p++;
        } else {
            current->type = TOKEN_INVALID;
            p++;
        }
        
        size_t len = p - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(current->value, start, len);
        current->value[len] = '\0';
        
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
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t%s\n", tokens[i].value);
    }
    
    return 0;
}