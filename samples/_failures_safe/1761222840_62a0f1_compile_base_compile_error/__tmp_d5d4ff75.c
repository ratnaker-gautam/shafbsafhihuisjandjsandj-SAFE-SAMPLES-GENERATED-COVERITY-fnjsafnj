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
           c == '.' || c == ':';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int scan_token(const char *input, int *pos, int line, int col, struct Token *token) {
    int start_pos = *pos;
    int length = strlen(input);
    
    if (start_pos >= length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    
    char current = input[start_pos];
    
    while (start_pos < length && is_whitespace(input[start_pos])) {
        if (input[start_pos] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        start_pos++;
    }
    
    if (start_pos >= length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = col;
        *pos = start_pos;
        return 1;
    }
    
    current = input[start_pos];
    *pos = start_pos;
    
    if (isdigit(current)) {
        int i = 0;
        while (*pos < length && i < 63 && isdigit(input[*pos])) {
            token->value[i++] = input[(*pos)++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = col;
        return 1;
    }
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while (*pos < length && i < 63 && (isalnum(input[*pos]) || input[*pos] == '_')) {
            token->value[i++] = input[(*pos)++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = col;
        return 1;
    }
    
    if (current == '"') {
        int i = 0;
        (*pos)++;
        while (*pos < length && i < 63 && input[*pos] != '"') {
            if (input[*pos] == '\\' && *pos + 1 < length) {
                token->value[i++] = input[(*pos)++];
            }
            token->value[i++] = input[(*pos)++];
        }
        if (*pos < length && input[*pos] == '"') {
            (*pos)++;
        } else {
            token->type = TOKEN_ERROR;
            strcpy(token->value, "Unterminated string");
            token->line = line;
            token->column = col;
            return 0;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = col;
        return 1;
    }
    
    if (is_operator_char(current)) {
        int i = 0;
        while (*pos < length && i < 63 && is_operator_char(input[*pos])) {
            token->value[i++] = input[(*pos)++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = col;
        return 1;
    }
    
    token->type = TOKEN_ERROR;
    snprintf(token->value, sizeof(token->value), "Unexpected character '%c'", current);
    token->line = line;
    token->column = col;
    (*pos)++;
    return 0;
}

const char* token_type_name(enum TokenType type) {
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
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input