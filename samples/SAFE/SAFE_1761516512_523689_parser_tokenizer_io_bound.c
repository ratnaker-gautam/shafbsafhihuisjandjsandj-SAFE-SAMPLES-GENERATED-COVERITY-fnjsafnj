//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
            continue;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) pos++;
            if (token_count >= max_tokens) break;
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, &input[start], pos - start);
            tokens[token_count].value[pos - start] = '\0';
            tokens[token_count].line = line;
            token_count++;
            continue;
        }
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (isalnum(input[pos]) || input[pos] == '_') pos++;
            if (token_count >= max_tokens) break;
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, &input[start], pos - start);
            tokens[token_count].value[pos - start] = '\0';
            tokens[token_count].line = line;
            token_count++;
            continue;
        }
        
        if (input[pos] == '"') {
            int start = pos;
            pos++;
            while (input[pos] != '\0' && input[pos] != '"') {
                if (input[pos] == '\n') line++;
                pos++;
            }
            if (input[pos] == '"') pos++;
            if (token_count >= max_tokens) break;
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, &input[start], pos - start);
            tokens[token_count].value[pos - start] = '\0';
            tokens[token_count].line = line;
            token_count++;
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            int start = pos;
            while (is_operator_char(input[pos])) pos++;
            if (token_count >= max_tokens) break;
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, &input[start], pos - start);
            tokens[token_count].value[pos - start] = '\0';
            tokens[token_count].line = line;
            token_count++;
            continue;
        }
        
        pos++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(enum TokenType type) {
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
    char input[1024];
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found (%d):\n", token_count);
    printf("Line | Type        | Value\n");
    printf("-----|-------------|-----------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%4d | %-11s | %s\n", 
               tokens[i].line, 
               token_type_name(tokens[i].type),
               tokens[i].value);
    }
    
    return 0;
}