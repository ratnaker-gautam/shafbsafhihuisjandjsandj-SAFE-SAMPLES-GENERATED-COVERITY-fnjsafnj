//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { NUMBER, OPERATOR, IDENTIFIER, END };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= 31) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (i >= 31) return 0;
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return i > 0;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    if (is_operator(input[*pos])) {
        buffer[0] = input[(*pos)++];
        buffer[1] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    while (*pos < len && isspace(input[*pos])) (*pos)++;
    
    if (*pos >= len) {
        token->type = END;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token->value)) {
        token->type = NUMBER;
        return 1;
    }
    
    if (parse_identifier(input, pos, token->value)) {
        token->type = IDENTIFIER;
        return 1;
    }
    
    if (parse_operator(input, pos, token->value)) {
        token->type = OPERATOR;
        return 1;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {"NUMBER", "OPERATOR", "IDENTIFIER", "END"};
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
            fprintf(stderr, "Invalid token at position %d\n", pos);
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