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
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
        }
        
        if (!*ptr) break;
        
        struct Token* token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(*ptr)) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
                column++;
            }
            token->value[i] = '\0';
        } else if (is_valid_identifier_start(*ptr)) {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while (is_valid_identifier_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
                column++;
            }
            token->value[i] = '\0';
        } else if (is_operator_char(*ptr)) {
            token->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
                column++;
            }
            token->value[i] = '\0';
        } else if (*ptr == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            token->value[i++] = *ptr++;
            column++;
            while (*ptr != '"' && *ptr && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
                column++;
            }
            if (*ptr == '"') {
                token->value[i++] = *ptr++;
                column++;
            }
            token->value[i] = '\0';
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = *ptr++;
            token->value[1] = '\0';
            column++;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
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
    printf("Token count: %d\n", count);
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-12s ", "");
        print_token_type(tokens[i].type);
        printf(" %-15s %-6d %-6d\n", tokens[i].value, tokens[i].line, tokens[i].column);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter input to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens,