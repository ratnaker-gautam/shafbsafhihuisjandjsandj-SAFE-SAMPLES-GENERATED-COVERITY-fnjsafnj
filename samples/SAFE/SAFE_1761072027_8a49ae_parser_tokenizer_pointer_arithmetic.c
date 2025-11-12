//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char *str) {
    if (isdigit(*str)) return TOKEN_NUMBER;
    if (*str == '"') return TOKEN_STRING;
    if (is_operator_char(*str)) return TOKEN_OPERATOR;
    if (isalpha(*str) || *str == '_') return TOKEN_IDENTIFIER;
    return TOKEN_ERROR;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        const char *start = p;
        TokenType type = classify_token(p);
        
        if (type == TOKEN_NUMBER) {
            while (isdigit(*p)) p++;
            if (*p == '.') {
                p++;
                while (isdigit(*p)) p++;
            }
        } else if (type == TOKEN_IDENTIFIER) {
            while (isalnum(*p) || *p == '_') p++;
        } else if (type == TOKEN_OPERATOR) {
            p++;
            if (is_operator_char(*p)) p++;
        } else if (type == TOKEN_STRING) {
            p++;
            while (*p && *p != '"') {
                if (*p == '\\' && *(p + 1)) p += 2;
                else p++;
            }
            if (*p == '"') p++;
            else return -1;
        } else {
            return -1;
        }
        
        size_t len = p - start;
        if (len >= MAX_TOKEN_LEN) return -1;
        
        tokens[token_count].type = type;
        strncpy(tokens[token_count].value, start, len);
        tokens[token_count].value[len] = '\0';
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
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count <= 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}