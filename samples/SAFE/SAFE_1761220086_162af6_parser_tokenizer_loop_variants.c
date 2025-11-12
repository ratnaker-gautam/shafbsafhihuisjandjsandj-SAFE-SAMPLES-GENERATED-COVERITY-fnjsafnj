//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, END };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isdigit(input[*pos])) {
        (*pos)++;
    }
    int len = *pos - start;
    if (len > 31) len = 31;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    int len = *pos - start;
    if (len > 31) len = 31;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = OPERATOR;
    (*pos)++;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = END;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        return parse_number(input, pos, token);
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        return parse_identifier(input, pos, token);
    }
    
    if (is_operator(input[*pos])) {
        return parse_operator(input, pos, token);
    }
    
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "END"};
    printf("Token: %-12s Value: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[256];
    printf("Enter expression: ");
    
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
    int token_count = 0;
    
    do {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid character at position %d: '%c'\n", pos, input[pos]);
            return 1;
        }
        
        if (token.type != END) {
            print_token(&token);
            token_count++;
        }
        
    } while (token.type != END);
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}