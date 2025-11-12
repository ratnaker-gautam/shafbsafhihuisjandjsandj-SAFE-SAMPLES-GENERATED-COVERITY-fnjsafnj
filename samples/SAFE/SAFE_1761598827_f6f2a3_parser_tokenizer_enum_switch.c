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
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
        if (input[*pos] == '\0') {
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            return 1;
        }
    }
    
    token->line = line;
    token->column = *col;
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*col)++;
    return 1;
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

void process_tokens(const char *input) {
    struct Token token;
    int pos = 0;
    int line = 1;
    int col = 1;
    
    printf("Input: %s\n", input);
    printf("Tokens:\n");
    
    while (get_token(&token, input, &pos, line, &col)) {
        print_token(&token);
        
        switch (token.type) {
            case TOKEN_NUMBER:
                break;
            case TOKEN_IDENTIFIER:
                break;
            case TOKEN_OPERATOR:
                break;
            case TOKEN_STRING:
                break;
            case TOKEN_EOF:
                return;
            case TOKEN_UNKNOWN:
                break;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression to tokenize: ");
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
    
    process_tokens(input);
    return 0;
}