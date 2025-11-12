//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    
    while (input[i] != '\0' && token_count < max_tokens) {
        if (isspace((unsigned char)input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit((unsigned char)input[i])) {
            token->type = TOKEN_NUMBER;
            int j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int j = 0;
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (is_operator_char(input[i])) {
            token->type = TOKEN_OPERATOR;
            int j = 0;
            while (is_operator_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (input[i] == '"') {
            token->type = TOKEN_STRING;
            int j = 0;
            token->value[j++] = input[i++];
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            if (input[i] == '"') {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else {
            i++;
            column++;
        }
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
    printf("%-12s %-16s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s %-16s %-6d %-6d\n",
               token_type_name(tokens[i].type),
               tokens[i].value,
               tokens[i].line,
               tokens[i].column);
    }
    
    return 0;
}