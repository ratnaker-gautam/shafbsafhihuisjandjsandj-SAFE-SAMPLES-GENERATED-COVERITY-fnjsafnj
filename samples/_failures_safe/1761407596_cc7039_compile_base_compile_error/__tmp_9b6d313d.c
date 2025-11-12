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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token create_token(enum TokenType type, const char* value, int line, int column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    return token;
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);

    while (i < input_len && token_count < max_tokens) {
        if (isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }

        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                number[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_NUMBER, number, line, column - j);
            continue;
        }

        if (isalpha(input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_IDENTIFIER, identifier, line, column - j);
            continue;
        }

        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int j = 0;
            string[j++] = input[i++];
            column++;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
                column++;
            }
            if (i < input_len && input[i] == '"') {
                string[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_STRING, string, line, column - j);
            continue;
        }

        if (is_operator_char(input[i])) {
            char op[3] = {0};
            op[0] = input[i++];
            column++;
            if (i < input_len && is_operator_char(input[i])) {
                op[1] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_OPERATOR, op, line, column - strlen(op));
            continue;
        }

        char unknown[2] = {input[i++], '\0'};
        tokens[token_count++] = create_token(TOKEN_UNKNOWN, unknown, line, column);
        column++;
    }

    if (token_count < max_tokens) {
        tokens[token_count++] = create_token(TOKEN_EOF, "", line, column);
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
    printf("%-12s %-15s %-8s %-8s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-12s ", "");
        print_token_type(tokens[i].type);
        printf(" %-15s %-8d %-8d\n", tokens[i].value, tokens[i].line, tokens[i].column);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >