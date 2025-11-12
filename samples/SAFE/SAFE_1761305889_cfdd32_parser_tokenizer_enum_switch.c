//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_END,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 31) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    (*pos)++;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    if (*pos - start > 31) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*pos)++;
    return 1;
}

int parse_paren(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '(' && input[*pos] != ')') return 0;
    
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = TOKEN_PAREN;
    (*pos)++;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_END;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    if (parse_paren(input, pos, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_PAREN:
            printf("PAREN: %s\n", token->value);
            break;
        case TOKEN_END:
            printf("END\n");
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    
    printf("Tokens:\n");
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Tokenization error\n");
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_END || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    return 0;
}