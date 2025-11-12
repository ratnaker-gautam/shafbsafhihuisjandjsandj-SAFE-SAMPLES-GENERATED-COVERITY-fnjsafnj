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

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);
    
    while (i < input_len && token_count < max_tokens) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (i >= input_len) break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(input[i])) {
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (is_valid_identifier_start(input[i])) {
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && is_valid_identifier_char(input[i])) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (input[i] == '"') {
            int j = 0;
            i++;
            column++;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(input[i])) {
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && is_operator_char(input[i])) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
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

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

void print_tokens(const struct Token* tokens, int count) {
    printf("Token count: %d\n", count);
    printf("Type\t\tValue\t\tLine\tColumn\n");
    printf("----\t\t-----\t\t----\t------\n");
    
    for (int i = 0; i < count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s", tokens[i].value);
        if (tokens[i].type == TOKEN_EOF) {
            printf("\t\t");
        } else if (strlen(tokens[i].value) < 8) {
            printf("\t\t");
        } else {
            printf("\t");
        }
        printf("%d\t%d\n", tokens[i].line, tokens[i].column);
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (max %d chars):\n", 1023);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1]