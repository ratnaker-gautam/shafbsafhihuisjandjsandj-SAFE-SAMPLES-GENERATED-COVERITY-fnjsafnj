//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int position;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(const char *input, int *pos) {
    struct Token token = {TOKEN_INVALID, "", *pos};
    int i = 0;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace(input[*pos])) {
        (*pos)++;
        token.position = *pos;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(input[*pos])) {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        token.value[i++] = input[*pos];
        (*pos)++;
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_INVALID;
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: %-10s Position: %d\n", 
           type_names[token.type], token.value, token.position);
}

void tokenize_recursive(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_EOF) {
        return;
    }
    
    print_token(token);
    
    if (token.type == TOKEN_INVALID) {
        printf("Error: Invalid token at position %d\n", token.position);
        return;
    }
    
    tokenize_recursive(input, pos);
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
    
    if (len == 1) {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    printf("\nToken stream:\n");
    tokenize_recursive(input, &pos);
    
    return 0;
}