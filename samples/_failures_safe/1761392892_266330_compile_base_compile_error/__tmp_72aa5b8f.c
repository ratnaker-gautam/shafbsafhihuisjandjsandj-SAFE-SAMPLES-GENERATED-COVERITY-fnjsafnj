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
        while (isspace(*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        tokens[token_idx].line = line;
        tokens[token_idx].column = column;
        
        if (isdigit(*ptr)) {
            tokens[token_idx].type = TOKEN_NUMBER;
            int len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            tokens[token_idx].value[len] = '\0';
            token_idx++;
        }
        else if (is_valid_identifier_start(*ptr)) {
            tokens[token_idx].type = TOKEN_IDENTIFIER;
            int len = 0;
            while (is_valid_identifier_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            tokens[token_idx].value[len] = '\0';
            token_idx++;
        }
        else if (is_operator_char(*ptr)) {
            tokens[token_idx].type = TOKEN_OPERATOR;
            int len = 0;
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            tokens[token_idx].value[len] = '\0';
            token_idx++;
        }
        else if (*ptr == '"') {
            tokens[token_idx].type = TOKEN_STRING;
            int len = 0;
            ptr++;
            column++;
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_idx].value[len++] = *ptr++;
                column++;
            }
            if (*ptr == '"') {
                ptr++;
                column++;
            }
            tokens[token_idx].value[len] = '\0';
            token_idx++;
        }
        else {
            ptr++;
            column++;
        }
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

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

void print_tokens(const struct Token* tokens, int count) {
    if (tokens == NULL || count <= 0) {
        return;
    }
    
    printf("Token count: %d\n", count);
    printf("%-12s %-15s %-8s %-8s\n", "Type", "Value", "Line", "Column");
    printf("------------ --------------- -------- --------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-12s ", "");
        print_token_type(tokens[i].type);
        printf(" %-15s %-8d %-8d\n", tokens[i].value, tokens[i].line, tokens[i].column);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {