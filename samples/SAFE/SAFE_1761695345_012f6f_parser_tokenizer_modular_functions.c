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
    TOKEN_INVALID
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
    
    while (input[i] != '\0' && token_count < max_tokens) {
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
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                number[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_NUMBER, number, line, column - j);
            continue;
        }
        
        if (isalpha(input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while ((isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                identifier[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_IDENTIFIER, identifier, line, column - j);
            continue;
        }
        
        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int j = 0;
            i++;
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                string[j++] = input[i++];
                column++;
            }
            if (input[i] == '"') {
                i++;
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_STRING, string, line, column - j - 1);
            continue;
        }
        
        if (is_operator_char(input[i])) {
            char op[3] = {0};
            op[0] = input[i];
            int j = 1;
            i++;
            column++;
            if (is_operator_char(input[i]) && j < 2) {
                op[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_OPERATOR, op, line, column - j);
            continue;
        }
        
        char invalid[2] = {input[i], '\0'};
        tokens[token_count++] = create_token(TOKEN_INVALID, invalid, line, column);
        i++;
        column++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count++] = create_token(TOKEN_EOF, "", line, column);
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        fprintf(stderr, "Input is empty\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}