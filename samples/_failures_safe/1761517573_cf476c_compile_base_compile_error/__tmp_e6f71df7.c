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
    
    while (input[pos] != '\0') {
        char c = input[pos];
        if (isdigit(c)) {
            if (len < 63) buffer[len++] = c;
        } else if (c == '.' && !has_dot) {
            if (len < 63) buffer[len++] = c;
            has_dot = 1;
        } else {
            break;
        }
        pos++;
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] != '\0' && (isalpha(input[pos]) || input[pos] == '_')) {
        if (len < 63) buffer[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) buffer[len++] = input[pos];
            pos++;
        }
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] == '"') {
        if (len < 63) buffer[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 63) buffer[len++] = input[pos];
            pos++;
        }
        
        if (input[pos] == '"') {
            if (len < 63) buffer[len++] = input[pos];
        }
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_ERROR, "", line, *column};
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = line;
    token.column = *column;
    
    char buffer[64];
    int advance = 0;
    
    if ((advance = parse_number(input, *pos, buffer)) > 0) {
        token.type = TOKEN_NUMBER;
        strncpy(token.value, buffer, 63);
        token.value[63] = '\0';
    } else if ((advance = parse_identifier(input, *pos, buffer)) > 0) {
        token.type = TOKEN_IDENTIFIER;
        strncpy(token.value, buffer, 63);
        token.value[63] = '\0';
    } else if ((advance = parse_operator(input, *pos, buffer)) > 0) {
        token.type = TOKEN_OPERATOR;
        strncpy(token.value, buffer, 63);
        token.value[63] = '\0';
    } else if ((advance = parse_string(input, *pos, buffer)) > 0) {
        token.type = TOKEN_STRING;
        strncpy(token.value, buffer, 63);
        token.value[63] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        advance = 1;
    }
    
    *pos += advance;
    *column += advance;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line