//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int line, int *col) {
    int start = *pos;
    int length = strlen(input);
    
    if (start >= length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = *col;
        return 1;
    }
    
    while (start < length && isspace(input[start])) {
        if (input[start] == '\n') {
            line++;
            *col = 1;
        } else {
            (*col)++;
        }
        start++;
    }
    
    *pos = start;
    if (*pos >= length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = *col;
        return 1;
    }
    
    char current = input[*pos];
    token->line = line;
    token->column = *col;
    
    if (isdigit(current)) {
        int i = 0;
        while (*pos < length && i < 31 && (isdigit(input[*pos]) || input[*pos] == '.')) {
            token->value[i++] = input[(*pos)++];
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (isalpha(current) || current == '_') {
        int i = 0;
        while (*pos < length && i < 31 && (isalnum(input[*pos]) || input[*pos] == '_')) {
            token->value[i++] = input[(*pos)++];
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (current == '"') {
        int i = 0;
        (*pos)++;
        (*col)++;
        while (*pos < length && i < 31 && input[*pos] != '"') {
            token->value[i++] = input[(*pos)++];
            (*col)++;
        }
        if (*pos < length && input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    } else if (is_operator_char(current)) {
        int i = 0;
        while (*pos < length && i < 31 && is_operator_char(input[*pos])) {
            token->value[i++] = input[(*pos)++];
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
    } else {
        token->value[0] = input[(*pos)++];
        token->value[1] = '\0';
        (*col)++;
        token->type = TOKEN_UNKNOWN;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
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
    int line = 1;
    int column = 1;
    struct Token token;
    
    printf("\nTokens:\n");
    printf("--------\n");
    
    while (1) {
        if (get_token(&token, input, &pos, line, &column) != 0) {
            break;
        }
        
        switch (token.type) {
            case TOKEN_NUMBER:
                print_token(&token);
                break;
            case TOKEN_IDENTIFIER:
                print_token(&token);
                break;
            case TOKEN_OPERATOR:
                print_token(&token);
                break;
            case TOKEN_STRING:
                print_token(&token);
                break;
            case TOKEN_EOF:
                print_token(&token);
                return 0;
            case TOKEN_UNKNOWN: