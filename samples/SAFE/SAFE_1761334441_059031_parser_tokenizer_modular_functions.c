//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        return TOKEN_NUMBER;
    } else if (isalpha(str[0]) || str[0] == '_') {
        return TOKEN_IDENTIFIER;
    } else if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    } else if (str[0] == '"') {
        return TOKEN_STRING;
    }
    return TOKEN_UNKNOWN;
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
        
        int start = pos;
        
        if (is_operator_char(input[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            token_count++;
            continue;
        }
        
        if (input[pos] == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int value_pos = 0;
            tokens[token_count].value[value_pos++] = input[pos++];
            
            while (pos < len && input[pos] != '"' && value_pos < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[value_pos++] = input[pos++];
            }
            
            if (pos < len && input[pos] == '"') {
                tokens[token_count].value[value_pos++] = input[pos++];
            }
            tokens[token_count].value[value_pos] = '\0';
            token_count++;
            continue;
        }
        
        if (isalnum(input[pos]) || input[pos] == '_') {
            int value_pos = 0;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && value_pos < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[value_pos++] = input[pos++];
            }
            tokens[token_count].value[value_pos] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
            token_count++;
            continue;
        }
        
        tokens[token_count].type = TOKEN_UNKNOWN;
        tokens[token_count].value[0] = input[pos];
        tokens[token_count].value[1] = '\0';
        pos++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

void display_tokens(const Token* tokens, int count) {
    printf("Tokenized output:\n");
    printf("%-15s %-15s\n", "Type", "Value");
    printf("--------------- ---------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-15s ", "");
        print_token_type(tokens[i].type);
        printf(" %-15s\n", tokens[i].value);
    }
}

int main() {
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Warning: Input too large, truncated tokenization\n");
    }
    
    display_tokens(tokens, token_count);
    
    return 0;
}