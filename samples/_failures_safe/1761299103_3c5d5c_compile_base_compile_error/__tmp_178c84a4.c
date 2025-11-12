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
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        char current = input[pos];
        
        if (isdigit(current)) {
            int start = pos;
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (is_valid_identifier_start(current)) {
            int start = pos;
            while (pos < len && is_valid_identifier_char(input[pos])) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator_char(current)) {
            tokens[token_count].value[0] = current;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
            pos++;
        }
        else if (current == '"') {
            int start = pos;
            pos++;
            while (pos < len && input[pos] != '"') {
                pos++;
            }
            if (pos < len && input[pos] == '"') {
                int token_len = pos - start - 1;
                if (token_len >= MAX_TOKEN_LEN) {
                    token_len = MAX_TOKEN_LEN - 1;
                }
                strncpy(tokens[token_count].value, &input[start + 1], token_len);
                tokens[token_count].value[token_len] = '\0';
                tokens[token_count].type = TOKEN_STRING;
                token_count++;
                pos++;
            } else {
                tokens[token_count].value[0] = current;
                tokens[token_count].value[1] = '\0';
                tokens[token_count].type = TOKEN_OPERATOR;
                token_count++;
                pos = start + 1;
            }
        }
        else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER:
            printf("NUMBER");
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        case TOKEN_STRING:
            printf("STRING");
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
    }
}

void print_tokens(const struct Token* tokens, int count) {
    printf("Tokenized output:\n");
    for (int i = 0; i < count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value=\"%s\"\n", tokens[i].value);
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TO