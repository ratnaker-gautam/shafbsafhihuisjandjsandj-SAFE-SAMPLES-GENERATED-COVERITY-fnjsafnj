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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        buffer[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            buffer[len++] = next;
        }
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *buffer) {
    int len = 0;
    int has_dot = 0;
    
    while (isdigit(input[pos]) || (input[pos] == '.' && !has_dot)) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 63) buffer[len++] = input[pos++];
        else break;
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (isalnum(input[pos]) || input[pos] == '_') {
            if (len < 63) buffer[len++] = input[pos++];
            else break;
        }
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] == '"') {
        buffer[len++] = input[pos++];
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < 62) buffer[len++] = input[pos++];
                if (len < 62) buffer[len++] = input[pos++];
            } else {
                if (len < 63) buffer[len++] = input[pos++];
                else break;
            }
        }
        
        if (input[pos] == '"' && len < 63) {
            buffer[len++] = input[pos];
        }
    }
    
    buffer[len] = '\0';
    return len > 1 ? len : 0;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_ERROR, "", line, *column};
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        int start_pos = *pos;
        int start_col = *column;
        int consumed = 0;
        
        if (parse_string(input, *pos, token.value)) {
            consumed = strlen(token.value);
            token.type = TOKEN_STRING;
        } else if (parse_number(input, *pos, token.value)) {
            consumed = strlen(token.value);
            token.type = TOKEN_NUMBER;
        } else if (parse_operator(input, *pos, token.value)) {
            consumed = strlen(token.value);
            token.type = TOKEN_OPERATOR;
        } else if (parse_identifier(input, *pos, token.value)) {
            consumed = strlen(token.value);
            token.type = TOKEN_IDENTIFIER;
        } else {
            token.value[0] = c;
            token.value[1] = '\0';
            consumed = 1;
            token.type = TOKEN_ERROR;
        }
        
        if (consumed > 0) {
            *pos += consumed;
            *column += consumed;
            token.line = line;
            token.column = start_col;
            return token;
        }
        
        (*pos)++;
        (*column)++;
    }
    
    token.type = TOKEN_EOF;
    token.value[0] = '\0';
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF && token.type != TOKEN_ERROR) {
        printf(" ('%s')", token.value);
    }
    
    printf("\n");
}

int main(void) {