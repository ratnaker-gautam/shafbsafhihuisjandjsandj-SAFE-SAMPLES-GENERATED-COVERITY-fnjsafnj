//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

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

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input) {
    if (input == NULL) return;
    
    size_t len = strlen(input);
    if (len == 0) return;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    int in_string = 0;
    
    for (size_t i = 0; i < len && token_count < MAX_TOKENS; i++) {
        char c = input[i];
        
        if (in_string) {
            if (c == '"') {
                if (buf_pos < MAX_TOKEN_LEN - 1) {
                    buffer[buf_pos] = '\0';
                    tokens[token_count].type = TOKEN_STRING;
                    strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                    token_count++;
                }
                buf_pos = 0;
                in_string = 0;
            } else {
                if (buf_pos < MAX_TOKEN_LEN - 1) {
                    buffer[buf_pos++] = c;
                }
            }
            continue;
        }
        
        if (isspace(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                if (isdigit(buffer[0])) {
                    tokens[token_count].type = TOKEN_NUMBER;
                } else {
                    tokens[token_count].type = TOKEN_IDENTIFIER;
                }
                
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            continue;
        }
        
        if (c == '"') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                tokens[token_count].type = TOKEN_IDENTIFIER;
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            in_string = 1;
            continue;
        }
        
        if (is_operator(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                tokens[token_count].type = TOKEN_IDENTIFIER;
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            
            if (token_count < MAX_TOKENS) {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value[0] = c;
                tokens[token_count].value[1] = '\0';
                token_count++;
            }
            continue;
        }
        
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = c;
        }
    }
    
    if (buf_pos > 0 && token_count < MAX_TOKENS) {
        buffer[buf_pos] = '\0';
        if (isdigit(buffer[0])) {
            tokens[token_count].type = TOKEN_NUMBER;
        } else {
            tokens[token_count].type = TOKEN_IDENTIFIER;
        }
        strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        token_count++;
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
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

void print_tokens() {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: Type=%s, Value='%s'\n", 
               i, token_type_to_string(tokens[i].type), tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");