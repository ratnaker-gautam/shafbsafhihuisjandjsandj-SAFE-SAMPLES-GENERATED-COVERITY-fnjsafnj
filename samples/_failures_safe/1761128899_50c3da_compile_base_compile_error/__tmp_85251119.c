//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens, int* token_count) {
    if (input == NULL || tokens == NULL || token_count == NULL) {
        return 0;
    }
    
    int line = 1;
    int column = 1;
    int token_idx = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_idx < max_tokens) {
        if (isspace(*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
            continue;
        }
        
        if (isdigit(*ptr)) {
            int len = 0;
            tokens[token_idx].line = line;
            tokens[token_idx].column = column;
            tokens[token_idx].type = TOKEN_NUMBER;
            
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            tokens[token_idx].value[len] = '\0';
            token_idx++;
            continue;
        }
        
        if (is_valid_identifier_start(*ptr)) {
            int len = 0;
            tokens[token_idx].line = line;
            tokens[token_idx].column = column;
            tokens[token_idx].type = TOKEN_IDENTIFIER;
            
            while (is_valid_identifier_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            tokens[token_idx].value[len] = '\0';
            token_idx++;
            continue;
        }
        
        if (is_operator_char(*ptr)) {
            tokens[token_idx].line = line;
            tokens[token_idx].column = column;
            tokens[token_idx].type = TOKEN_OPERATOR;
            tokens[token_idx].value[0] = *ptr;
            tokens[token_idx].value[1] = '\0';
            token_idx++;
            ptr++;
            column++;
            continue;
        }
        
        if (*ptr == '"') {
            int len = 0;
            tokens[token_idx].line = line;
            tokens[token_idx].column = column;
            tokens[token_idx].type = TOKEN_STRING;
            tokens[token_idx].value[len++] = *ptr++;
            column++;
            
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 2) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            
            if (*ptr == '"') {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
                tokens[token_idx].value[len] = '\0';
                token_idx++;
            } else {
                return 0;
            }
            continue;
        }
        
        ptr++;
        column++;
    }
    
    if (token_idx < max_tokens) {
        tokens[token_idx].type = TOKEN_EOF;
        tokens[token_idx].value[0] = '\0';
        tokens[token_idx].line = line;
        tokens[token_idx].column = column;
        token_idx++;
    }
    
    *token_count = token_idx;
    return 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(const struct Token* tokens, int token_count) {
    if (tokens == NULL || token_count <= 0) {
        return;
    }
    
    printf("Tokens found:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s %-15s %-6d %-6d\n",
               token_type_to_string(tokens[i].type),
               tokens[i].value,
               tokens[i].line,
               tokens[i].column);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input