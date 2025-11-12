//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    char value[32];
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
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens - 1) {
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
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(*ptr)) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*ptr) && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        }
        else if (is_valid_identifier_start(*ptr)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int i = 0;
            while (is_valid_identifier_char(*ptr) && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            column++;
            token_count++;
        }
        else if (*ptr == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            ptr++;
            column++;
            while (*ptr != '"' && *ptr && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
                column++;
            }
            if (*ptr == '"') {
                ptr++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        }
        else {
            tokens[token_count].type = TOKEN_INVALID;
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            column++;
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    token_count++;
    
    return token_count;
}

void print_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s", token.value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"", token.value);
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s", token.value);
            break;
    }
    printf(" (line %d, column %d)\n", token.line, token.column);
}

int main() {
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
    
    struct Token tokens[256];
    int token_count = tokenize(input, tokens, 256);
    
    if (token_count >= 256) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%d. ", i + 1);
        print_token(tokens[i]);
    }
    
    return 0;
}