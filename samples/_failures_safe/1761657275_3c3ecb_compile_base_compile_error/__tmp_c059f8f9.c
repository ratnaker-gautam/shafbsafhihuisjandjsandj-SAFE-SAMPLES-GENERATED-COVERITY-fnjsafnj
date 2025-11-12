//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_UNKNOWN;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    size_t input_len = strlen(input);
    
    if (input_len == 0) return 0;
    
    for (size_t i = 0; i < input_len && token_count < MAX_TOKENS; i++) {
        if (strchr(DELIMITERS, input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            continue;
        }
        
        if (is_operator(input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            buffer[0] = input[i];
            buffer[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
            continue;
        }
        
        if (input[i] == '"') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            buffer[0] = input[i];
            buf_pos = 1;
            i++;
            while (i < input_len && input[i] != '"' && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = input[i++];
            }
            if (i < input_len && input[i] == '"') {
                buffer[buf_pos++] = input[i];
            }
            buffer[buf_pos] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
            buf_pos = 0;
            continue;
        }
        
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = input[i];
        }
    }
    
    if (buf_pos > 0 && token_count < MAX_TOKENS) {
        buffer[buf_pos] = '\0';
        tokens[token_count].type = classify_token(buffer);
        strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        token_count++;
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return 1;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) ==