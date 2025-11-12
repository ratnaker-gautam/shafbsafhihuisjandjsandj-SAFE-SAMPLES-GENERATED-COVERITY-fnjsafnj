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
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_token(const char *input, int pos, struct Token *token) {
    int start = pos;
    int len = strlen(input);
    
    while (pos < len && isspace(input[pos])) {
        pos++;
    }
    
    if (pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return pos;
    }
    
    token->line = 1;
    token->column = pos + 1;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        int i = 0;
        while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && i < 31) {
            token->value[i++] = input[pos++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (isdigit(input[pos])) {
        int i = 0;
        while (pos < len && (isdigit(input[pos]) || input[pos] == '.') && i < 31) {
            token->value[i++] = input[pos++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (input[pos] == '"') {
        int i = 0;
        pos++;
        while (pos < len && input[pos] != '"' && i < 31) {
            token->value[i++] = input[pos++];
        }
        if (pos < len && input[pos] == '"') {
            pos++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    } else if (is_operator_char(input[pos])) {
        int i = 0;
        while (pos < len && is_operator_char(input[pos]) && i < 31) {
            token->value[i++] = input[pos++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
    } else {
        token->value[0] = input[pos];
        token->value[1] = '\0';
        token->type = TOKEN_INVALID;
        pos++;
    }
    
    return pos;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s (line %d, column %d)\n", token->value, token->line, token->column);
            break;
        case TOKEN_NUMBER:
            printf("NUMBER: %s (line %d, column %d)\n", token->value, token->line, token->column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s (line %d, column %d)\n", token->value, token->line, token->column);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\" (line %d, column %d)\n", token->value, token->line, token->column);
            break;
        case TOKEN_EOF:
            printf("EOF (line %d, column %d)\n", token->line, token->column);
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s (line %d, column %d)\n", token->value, token->line, token->column);
            break;
    }
}

int main(void) {
    char input[1024];
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