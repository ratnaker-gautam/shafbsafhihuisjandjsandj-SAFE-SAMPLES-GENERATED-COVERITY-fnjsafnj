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
    
    if (c == '&' && input[pos + 1] == '&') {
        buffer[len++] = '&';
        buffer[len++] = '&';
    } else if (c == '|' && input[pos + 1] == '|') {
        buffer[len++] = '|';
        buffer[len++] = '|';
    } else if (c == '=' && input[pos + 1] == '=') {
        buffer[len++] = '=';
        buffer[len++] = '=';
    } else if (c == '!' && input[pos + 1] == '=') {
        buffer[len++] = '!';
        buffer[len++] = '=';
    } else if (c == '<' && input[pos + 1] == '=') {
        buffer[len++] = '<';
        buffer[len++] = '=';
    } else if (c == '>' && input[pos + 1] == '=') {
        buffer[len++] = '>';
        buffer[len++] = '=';
    } else if (is_operator_char(c)) {
        buffer[len++] = c;
    }
    
    buffer[len] = '\0';
    return len;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_ERROR, "", line, *column};
    int max_len = sizeof(token.value) - 1;
    
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
        token.value[0] = '\0';
        return token;
    }
    
    token.line = line;
    token.column = *column;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        int len = 0;
        while (input[*pos] != '\0' && isdigit(input[*pos]) && len < max_len) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[len] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        int len = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_') && len < max_len) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[len] = '\0';
    } else if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        int len = 0;
        (*pos)++;
        (*column)++;
        while (input[*pos] != '\0' && input[*pos] != '"' && len < max_len) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*column)++;
        } else {
            token.type = TOKEN_ERROR;
        }
        token.value[len] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        int op_len = parse_operator(input, *pos, token.value);
        if (op_len > 0) {
            *pos += op_len;
            *column += op_len;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
        (*column)++;
    }
    
    return token;
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
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {