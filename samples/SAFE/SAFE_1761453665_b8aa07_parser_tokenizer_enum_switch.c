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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_INVALID, "", *line, *col};
    int i = 0;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    token.line = *line;
    token.column = *col;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        token.value[i++] = input[*pos];
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        
        if (input[*pos] == '"') {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
            token.value[i] = '\0';
        } else {
            token.type = TOKEN_INVALID;
        }
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_INVALID;
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*col)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        
        switch (token.type) {
            case TOKEN_NUMBER:
            case TOKEN_IDENTIFIER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
                print_token(token);
                break;
                
            case TOKEN_EOF:
                print_token(token);
                return 0;
                
            case TOKEN_INVALID:
                print_token(token);
                break;
        }
        
        if (pos >= 1023) {
            break;
        }
    }
    
    return 0;
}