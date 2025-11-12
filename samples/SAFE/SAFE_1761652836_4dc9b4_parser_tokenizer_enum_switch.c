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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int get_token(struct Token *token, const char *input, int *pos, int line, int column) {
    int start = *pos;
    int len = strlen(input);
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    char c = input[start];
    
    while (isspace(c)) {
        start++;
        if (start >= len) {
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            *pos = start;
            return 1;
        }
        c = input[start];
        column++;
    }
    
    token->line = line;
    token->column = column;
    
    if (isdigit(c)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (start < len && i < 31 && isdigit(input[start])) {
            token->value[i++] = input[start++];
            column++;
        }
        token->value[i] = '\0';
        *pos = start;
        return 1;
    }
    
    if (isalpha(c) || c == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (start < len && i < 31 && (isalnum(input[start]) || input[start] == '_')) {
            token->value[i++] = input[start++];
            column++;
        }
        token->value[i] = '\0';
        *pos = start;
        return 1;
    }
    
    if (c == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        start++;
        column++;
        while (start < len && i < 31 && input[start] != '"') {
            token->value[i++] = input[start++];
            column++;
        }
        if (start < len && input[start] == '"') {
            start++;
            column++;
        }
        token->value[i] = '\0';
        *pos = start;
        return 1;
    }
    
    if (is_operator_char(c)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (start < len && i < 31 && is_operator_char(input[start])) {
            token->value[i++] = input[start++];
            column++;
        }
        token->value[i] = '\0';
        *pos = start;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = c;
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
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
    
    if (len == 1 && input[0] == '\n') {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("Tokens:\n");
    
    while (1) {
        struct Token token;
        if (!get_token(&token, input, &pos, line, column)) {
            fprintf(stderr, "Error getting token\n");
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        column = token.column + (int)strlen(token.value);
    }
    
    return 0;
}