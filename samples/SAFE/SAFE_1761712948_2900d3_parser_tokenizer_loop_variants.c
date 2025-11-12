//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_END };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_digit = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            has_digit = 1;
            (*pos)++;
        } else if (input[*pos] == '.' && isdigit(input[*pos + 1])) {
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (!has_digit) return 0;
    
    int len = *pos - start;
    if (len >= 32) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    (*pos)++;
    while (input[*pos] != '\0') {
        if (!isalnum(input[*pos]) && input[*pos] != '_') break;
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len >= 32) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
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

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    skip_whitespace(input, pos);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    int i = 0;
    while (i < 3) {
        int success = 0;
        switch (i) {
            case 0: success = parse_number(input, pos, token); break;
            case 1: success = parse_identifier(input, pos, token); break;
            case 2: success = parse_operator(input, pos, token); break;
        }
        if (success) return 1;
        i++;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "END"};
    printf("%s: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[256];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
    int token_count = 0;
    
    do {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        
        if (token.type != TOKEN_END) {
            print_token(&token);
            token_count++;
        }
        
    } while (token.type != TOKEN_END);
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}