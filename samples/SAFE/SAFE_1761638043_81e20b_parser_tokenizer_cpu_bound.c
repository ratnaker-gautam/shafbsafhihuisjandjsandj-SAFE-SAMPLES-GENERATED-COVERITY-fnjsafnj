//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 100

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (input[pos] == '"') {
            int start = pos;
            pos++;
            while (pos < len && input[pos] != '"') {
                pos++;
            }
            if (pos < len) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(input[pos])) {
            int start = pos;
            while (pos < len && is_operator_char(input[pos])) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else {
            pos++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
    
    return token_count;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[4096];
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
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s\t%s\n", token_type_to_string(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}