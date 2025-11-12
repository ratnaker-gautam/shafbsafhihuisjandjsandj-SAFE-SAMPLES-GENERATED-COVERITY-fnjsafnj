//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|' || c == '^' || c == '%' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';' ||
           c == '.' || c == ':' || c == '?';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int start_pos = *pos;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (is_whitespace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
        start_pos = *pos;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = *line;
    token.column = *col;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_ERROR;
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*col)++;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    
    printf("\nTokens:\n");
    printf("Line Col Type        Value\n");
    printf("---- --- ----------  --------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        printf