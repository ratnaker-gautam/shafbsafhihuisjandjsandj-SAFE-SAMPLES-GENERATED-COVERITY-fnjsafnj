//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    int i = 0;
    int has_dot = 0;
    while (*pos < len && i < buffer_size - 1) {
        char c = input[*pos];
        if (isdigit(c)) {
            buffer[i++] = c;
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            buffer[i++] = c;
            (*pos)++;
            has_dot = 1;
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_string(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    if (*pos >= len || input[*pos] != '"') return 0;
    
    int i = 0;
    (*pos)++;
    while (*pos < len && i < buffer_size - 1) {
        char c = input[*pos];
        if (c == '"') {
            (*pos)++;
            buffer[i] = '\0';
            return 1;
        }
        if (c == '\\') {
            (*pos)++;
            if (*pos >= len) break;
            c = input[*pos];
        }
        buffer[i++] = c;
        (*pos)++;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    if (*pos >= len || !is_valid_identifier_start(input[*pos])) return 0;
    
    int i = 0;
    while (*pos < len && i < buffer_size - 1 && is_valid_identifier_char(input[*pos])) {
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_operator(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    
    int i = 0;
    while (*pos < len && i < buffer_size - 1 && is_operator_char(input[*pos])) {
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

void skip_whitespace(const char *input, int *pos, int len, int *line, int *column) {
    while (*pos < len) {
        char c = input[*pos];
        if (c == ' ' || c == '\t') {
            (*pos)++;
            (*column)++;
        } else if (c == '\n') {
            (*pos)++;
            (*line)++;
            *column = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(const char *input, int *pos, int len, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    
    skip_whitespace(input, pos, len, line, column);
    token.line = *line;
    token.column = *column;
    
    if (*pos >= len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = input[*pos];
    
    if (parse_string(input, pos, len, token.value, sizeof(token.value))) {
        token.type = TOKEN_STRING;
    } else if (parse_number(input, pos, len, token.value, sizeof(token.value))) {
        token.type = TOKEN_NUMBER;
    } else if (parse_identifier(input, pos, len, token.value, sizeof(token.value))) {
        token.type = TOKEN_IDENTIFIER;
    } else if (parse_operator(input, pos, len, token.value, sizeof(token.value))) {
        token.type = TOKEN_OPERATOR;
    } else {
        token.value[0] = c;
        token.value[1] = '\0';
        (*pos)++;
        (*column)++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "ERROR";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input,