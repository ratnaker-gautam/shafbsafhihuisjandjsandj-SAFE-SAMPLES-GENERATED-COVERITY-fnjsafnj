//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

struct Token get_next_token(const char* input, int* pos, int* line, int* col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int input_len = (int)strlen(input);
    
    if (*pos >= input_len) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    while (*pos < input_len && is_whitespace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (*pos >= input_len) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    token.line = *line;
    token.column = *col;
    char current = input[*pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int value_len = 0;
        
        while (*pos < input_len && value_len < 63 && 
               (isdigit(input[*pos]) || input[*pos] == '.')) {
            token.value[value_len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[value_len] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int value_len = 0;
        
        while (*pos < input_len && value_len < 63 && 
               (isalnum(input[*pos]) || input[*pos] == '_')) {
            token.value[value_len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[value_len] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int value_len = 0;
        (*pos)++;
        (*col)++;
        
        while (*pos < input_len && value_len < 63 && input[*pos] != '"') {
            if (input[*pos] == '\\' && *pos + 1 < input_len) {
                (*pos)++;
                (*col)++;
                token.value[value_len++] = input[*pos];
            } else {
                token.value[value_len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        
        if (*pos < input_len && input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token.value[value_len] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int value_len = 0;
        
        while (*pos < input_len && value_len < 63 && is_operator_char(input[*pos])) {
            token.value[value_len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[value_len] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
        (*col)++;
    }
    
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
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("