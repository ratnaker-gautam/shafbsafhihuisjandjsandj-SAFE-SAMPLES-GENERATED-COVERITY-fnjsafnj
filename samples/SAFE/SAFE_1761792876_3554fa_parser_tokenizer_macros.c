//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;

void tokenize(const char* input) {
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i < input_len && token_count < MAX_TOKENS; i++) {
        if (strchr(DELIMITERS, input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                TokenType type = TOKEN_UNKNOWN;
                
                if (isdigit(buffer[0])) {
                    type = TOKEN_NUMBER;
                } else if (strchr("+-*/", buffer[0]) && buf_pos == 1) {
                    type = TOKEN_OPERATOR;
                } else if (isalpha(buffer[0])) {
                    type = TOKEN_IDENTIFIER;
                }
                
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                tokens[token_count].type = type;
                token_count++;
                buf_pos = 0;
            }
            continue;
        }
        
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = input[i];
        }
    }
    
    if (buf_pos > 0 && token_count < MAX_TOKENS) {
        buffer[buf_pos] = '\0';
        TokenType type = TOKEN_UNKNOWN;
        
        if (isdigit(buffer[0])) {
            type = TOKEN_NUMBER;
        } else if (strchr("+-*/", buffer[0]) && buf_pos == 1) {
            type = TOKEN_OPERATOR;
        } else if (isalpha(buffer[0])) {
            type = TOKEN_IDENTIFIER;
        }
        
        strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        tokens[token_count].type = type;
        token_count++;
    }
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s (%s)\n", i + 1, tokens[i].value, 
               token_type_to_string(tokens[i].type));
    }
    
    return 0;
}