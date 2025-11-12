//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_token(const char *input, int pos, struct Token *token) {
    int len = strlen(input);
    if (pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return pos;
    }

    char c = input[pos];
    
    if (isspace(c)) {
        return parse_token(input, pos + 1, token);
    }
    
    if (isalpha(c) || c == '_') {
        int i = 0;
        while (pos < len && i < 31 && (isalnum(input[pos]) || input[pos] == '_')) {
            token->value[i++] = input[pos++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return pos;
    }
    
    if (isdigit(c)) {
        int i = 0;
        while (pos < len && i < 31 && isdigit(input[pos])) {
            token->value[i++] = input[pos++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        return pos;
    }
    
    if (c == '"') {
        int i = 0;
        pos++;
        while (pos < len && i < 31 && input[pos] != '"') {
            token->value[i++] = input[pos++];
        }
        if (pos < len && input[pos] == '"') {
            pos++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return pos;
    }
    
    if (is_operator_char(c)) {
        token->value[0] = c;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        return pos + 1;
    }
    
    token->value[0] = c;
    token->value[1] = '\0';
    token->type = TOKEN_INVALID;
    return pos + 1;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token->value);
            break;
    }
}

int main(void) {
    char input[256];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    
    printf("Tokens:\n");
    do {
        pos = parse_token(input, pos, &token);
        print_token(&token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    return 0;
}