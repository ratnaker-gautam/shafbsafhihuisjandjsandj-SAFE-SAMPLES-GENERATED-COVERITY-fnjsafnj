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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int line, int column) {
    int start_pos = *pos;
    int len = strlen(input);
    
    if (start_pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = column;
        return 1;
    }
    
    char c = input[start_pos];
    
    if (isspace(c)) {
        while (start_pos < len && isspace(input[start_pos])) {
            if (input[start_pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            start_pos++;
        }
        *pos = start_pos;
        return get_token(token, input, pos, line, column);
    }
    
    if (isdigit(c)) {
        int i = 0;
        while (start_pos < len && i < 31 && isdigit(input[start_pos])) {
            token->value[i++] = input[start_pos++];
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = column - i;
        *pos = start_pos;
        return 1;
    }
    
    if (isalpha(c) || c == '_') {
        int i = 0;
        while (start_pos < len && i < 31 && (isalnum(input[start_pos]) || input[start_pos] == '_')) {
            token->value[i++] = input[start_pos++];
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = column - i;
        *pos = start_pos;
        return 1;
    }
    
    if (c == '"') {
        int i = 0;
        start_pos++;
        column++;
        while (start_pos < len && i < 31 && input[start_pos] != '"') {
            token->value[i++] = input[start_pos++];
            column++;
        }
        if (start_pos < len && input[start_pos] == '"') {
            start_pos++;
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = column - i - 1;
        *pos = start_pos;
        return 1;
    }
    
    if (is_operator_char(c)) {
        int i = 0;
        while (start_pos < len && i < 31 && is_operator_char(input[start_pos])) {
            token->value[i++] = input[start_pos++];
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = column - i;
        *pos = start_pos;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = c;
    token->value[1] = '\0';
    token->line = line;
    token->column = column;
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
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
        
        line = token.line;
        column = token.column + (int)strlen(token.value);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {