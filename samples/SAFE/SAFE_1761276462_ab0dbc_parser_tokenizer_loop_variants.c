//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_IDENTIFIER, TOKEN_END };

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
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        return parse_number(input, pos, token);
    }
    
    if (is_operator(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        return parse_identifier(input, pos, token);
    }
    
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "OPERATOR", "IDENTIFIER", "END"};
    printf("Token: %-12s Value: %s\n", type_names[token->type], token->value);
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    
    printf("\nTokens:\n");
    printf("---------------\n");
    
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_END) {
            break;
        }
    }
    
    return 0;
}