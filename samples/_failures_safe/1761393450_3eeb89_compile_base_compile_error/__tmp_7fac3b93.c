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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        buffer[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            buffer[len++] = input[pos + 1];
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *buffer) {
    int len = 0;
    int has_dot = 0;
    
    while (isdigit(input[pos]) || (input[pos] == '.' && !has_dot)) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 31) buffer[len++] = input[pos++];
        else break;
    }
    buffer[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (isalnum(input[pos]) || input[pos] == '_') {
            if (len < 31) buffer[len++] = input[pos++];
            else break;
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] == '"') {
        pos++;
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 31) buffer[len++] = input[pos++];
            else break;
        }
        if (input[pos] == '"') {
            buffer[len] = '\0';
            return len + 2;
        }
    }
    return 0;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    int start_pos = *pos;
    int token_len = 0;
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
        start_pos = *pos;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.column = *column;
    
    if (isdigit(input[*pos])) {
        token_len = parse_number(input, *pos, token.value);
        if (token_len > 0) {
            token.type = TOKEN_NUMBER;
            *pos += token_len;
            *column += token_len;
            return token;
        }
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token_len = parse_identifier(input, *pos, token.value);
        if (token_len > 0) {
            token.type = TOKEN_IDENTIFIER;
            *pos += token_len;
            *column += token_len;
            return token;
        }
    }
    
    if (is_operator_char(input[*pos])) {
        token_len = parse_operator(input, *pos, token.value);
        if (token_len > 0) {
            token.type = TOKEN_OPERATOR;
            *pos += token_len;
            *column += token_len;
            return token;
        }
    }
    
    if (input[*pos] == '"') {
        token_len = parse_string(input, *pos, token.value);
        if (token_len > 0) {
            token.type = TOKEN_STRING;
            *pos += token_len;
            *column += token_len;
            return token;
        }
    }
    
    token.type = TOKEN_INVALID;
    if (input[*pos] != '\0') {
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
        (*column)++;
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
        case TOKEN_INVALID: return "INVALID";
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
    if (len > 0 && input[len