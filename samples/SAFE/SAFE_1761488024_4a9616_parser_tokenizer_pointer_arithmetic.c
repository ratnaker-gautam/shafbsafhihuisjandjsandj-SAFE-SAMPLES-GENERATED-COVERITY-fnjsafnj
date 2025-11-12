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

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            char *start = (char*)p;
            while (isdigit(*p)) p++;
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_NUMBER;
                size_t len = p - start;
                if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
                strncpy(tokens[token_count].value, start, len);
                tokens[token_count].value[len] = '\0';
                token_count++;
            }
        }
        else if (isalpha(*p) || *p == '_') {
            char *start = (char*)p;
            while (isalnum(*p) || *p == '_') p++;
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_IDENTIFIER;
                size_t len = p - start;
                if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
                strncpy(tokens[token_count].value, start, len);
                tokens[token_count].value[len] = '\0';
                token_count++;
            }
        }
        else if (is_operator_char(*p)) {
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value[0] = *p;
                tokens[token_count].value[1] = '\0';
                token_count++;
            }
            p++;
        }
        else if (*p == '"') {
            char *start = (char*)p;
            p++;
            while (*p && *p != '"') p++;
            if (*p == '"') {
                if (token_count < max_tokens) {
                    tokens[token_count].type = TOKEN_STRING;
                    size_t len = p - start;
                    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
                    strncpy(tokens[token_count].value, start, len);
                    tokens[token_count].value[len] = '\0';
                    token_count++;
                }
                p++;
            } else {
                if (token_count < max_tokens) {
                    tokens[token_count].type = TOKEN_ERROR;
                    strcpy(tokens[token_count].value, "Unterminated string");
                    token_count++;
                }
                break;
            }
        }
        else {
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_ERROR;
                tokens[token_count].value[0] = *p;
                tokens[token_count].value[1] = '\0';
                token_count++;
            }
            p++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("Empty input\n");
        return 0;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s\t%s\n", token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}