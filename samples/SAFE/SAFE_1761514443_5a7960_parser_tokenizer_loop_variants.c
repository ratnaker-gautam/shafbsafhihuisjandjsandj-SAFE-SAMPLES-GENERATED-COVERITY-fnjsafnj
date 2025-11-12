//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(**input)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.value[0] = **input;
    token.value[1] = '\0';
    (*input)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char *cursor = input;
    struct Token token;
    
    printf("\nToken stream:\n");
    do {
        token = get_next_token(&cursor);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    int token_count = 0;
    cursor = input;
    while (1) {
        token = get_next_token(&cursor);
        if (token.type == TOKEN_EOF) break;
        if (token.type == TOKEN_INVALID) break;
        token_count++;
    }
    
    printf("\nTotal valid tokens: %d\n", token_count);
    
    cursor = input;
    struct Token tokens[100];
    int i = 0;
    for (i = 0; i < 100; i++) {
        tokens[i] = get_next_token(&cursor);
        if (tokens[i].type == TOKEN_EOF || tokens[i].type == TOKEN_INVALID) {
            break;
        }
    }
    
    printf("\nToken array contents:\n");
    for (int j = 0; j < i; j++) {
        print_token(tokens[j]);
    }
    
    return 0;
}