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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token create_token(enum TokenType type, const char* value, int line) {
    struct Token token;
    token.type = type;
    token.line = line;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    return token;
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (isspace(*pos)) {
            if (*pos == '\n') line++;
            pos++;
        }
        
        if (!*pos) break;
        
        if (isdigit(*pos)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int i = 0;
            while (isdigit(*pos) && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *pos++;
            }
            tokens[token_count++] = create_token(TOKEN_NUMBER, buffer, line);
        }
        else if (isalpha(*pos) || *pos == '_') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int i = 0;
            while ((isalnum(*pos) || *pos == '_') && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *pos++;
            }
            tokens[token_count++] = create_token(TOKEN_IDENTIFIER, buffer, line);
        }
        else if (*pos == '"') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int i = 0;
            pos++;
            while (*pos && *pos != '"' && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *pos++;
            }
            if (*pos == '"') {
                pos++;
                tokens[token_count++] = create_token(TOKEN_STRING, buffer, line);
            } else {
                tokens[token_count++] = create_token(TOKEN_UNKNOWN, buffer, line);
            }
        }
        else if (is_operator_char(*pos)) {
            char buffer[2] = {*pos, '\0'};
            tokens[token_count++] = create_token(TOKEN_OPERATOR, buffer, line);
            pos++;
        }
        else {
            char buffer[2] = {*pos, '\0'};
            tokens[token_count++] = create_token(TOKEN_UNKNOWN, buffer, line);
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count] = create_token(TOKEN_EOF, "", line);
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
    printf("%-12s %-15s %s\n", "Line", "Type", "Value");
    printf("------------ --------------- --------------------\n");
    
    for (int i = 0; i < count && i < MAX_TOKENS; i++) {
        printf("%-12d ", tokens[i].line);
        print_token_type(tokens[i].type);
        printf(" %-15s\n", tokens[i].value);
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        printf("Warning: Maximum token count reached\n");
    }
    
    print_tokens(tokens, token_count);
    
    return 0;
}