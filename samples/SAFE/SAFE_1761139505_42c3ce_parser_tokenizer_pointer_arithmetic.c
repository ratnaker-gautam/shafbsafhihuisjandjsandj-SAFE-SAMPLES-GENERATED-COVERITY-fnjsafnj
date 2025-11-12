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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_tokens(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isalpha(*p) || *p == '_') {
            char *token_ptr = tokens[token_count].value;
            while ((isalnum(*p) || *p == '_') && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (isdigit(*p)) {
            char *token_ptr = tokens[token_count].value;
            while (isdigit(*p) && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*p == '"') {
            char *token_ptr = tokens[token_count].value;
            p++;
            while (*p && *p != '"' && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            if (*p == '"') {
                p++;
                *token_ptr = '\0';
                tokens[token_count].type = TOKEN_STRING;
                token_count++;
            } else {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "Unterminated string");
                token_count++;
                break;
            }
        }
        else if (is_operator_char(*p)) {
            char *token_ptr = tokens[token_count].value;
            *token_ptr++ = *p++;
            if (is_operator_char(*p) && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            tokens[token_count].type = TOKEN_ERROR;
            char *token_ptr = tokens[token_count].value;
            *token_ptr++ = *p++;
            *token_ptr = '\0';
            token_count++;
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = parse_tokens(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found: %d\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}