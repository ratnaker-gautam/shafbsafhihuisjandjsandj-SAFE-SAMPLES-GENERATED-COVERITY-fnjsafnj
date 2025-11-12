//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN + 1];
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

void tokenize(const char* input, struct Token* tokens, int* token_count) {
    int i = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[i] != '\0' && *token_count < MAX_TOKENS) {
        while (isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (input[i] == '\0') break;
        
        struct Token token;
        token.line = line;
        token.column = column;
        token.value[0] = '\0';
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            int j = 0;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        }
        else if (is_valid_identifier_start(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            int j = 0;
            while (is_valid_identifier_char(input[i]) && j < MAX_TOKEN_LEN) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        }
        else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            int j = 0;
            while (is_operator_char(input[i]) && j < MAX_TOKEN_LEN) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        }
        else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            int j = 0;
            token.value[j++] = input[i++];
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN) {
                token.value[j++] = input[i++];
                column++;
            }
            if (input[i] == '"') {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        }
        else {
            token.type = TOKEN_INVALID;
            token.value[0] = input[i++];
            token.value[1] = '\0';
            column++;
        }
        
        tokens[(*token_count)++] = token;
    }
    
    if (*token_count < MAX_TOKENS) {
        struct Token eof_token;
        eof_token.type = TOKEN_EOF;
        eof_token.value[0] = '\0';
        eof_token.line = line;
        eof_token.column = column;
        tokens[(*token_count)++] = eof_token;
    }
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);
    
    printf("\nTokens found:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s %-15s %-6d %-6d\n",
               token_type_to_string(tokens[i].type),
               tokens[i].value,
               tokens[i].line,