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
    TOKEN_UNKNOWN
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

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    char c = input[pos];
    if (c == '\0') return 0;
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    while (input[pos] != '\0' && (isdigit(input[pos]) || (input[pos] == '.' && !has_dot))) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 31) output[len++] = input[pos++];
        else break;
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (input[pos] != '\0' && (isalpha(input[pos]) || input[pos] == '_')) {
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 31) output[len++] = input[pos++];
            else break;
        }
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (input[pos] == '"') {
        pos++;
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 31) output[len++] = input[pos++];
            else break;
        }
        if (input[pos] == '"') {
            output[len] = '\0';
            return len + 2;
        }
    }
    return 0;
}

int get_next_token(const char *input, int pos, struct Token *token, int line, int column) {
    if (input == NULL || token == NULL || pos < 0) return -1;
    
    while (input[pos] != '\0' && isspace(input[pos])) {
        if (input[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
    
    if (input[pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = column;
        return pos;
    }
    
    token->line = line;
    token->column = column;
    
    int advance = parse_number(input, pos, token->value);
    if (advance > 0) {
        token->type = TOKEN_NUMBER;
        return pos + advance;
    }
    
    advance = parse_identifier(input, pos, token->value);
    if (advance > 0) {
        token->type = TOKEN_IDENTIFIER;
        return pos + advance;
    }
    
    advance = parse_operator(input, pos, token->value);
    if (advance > 0) {
        token->type = TOKEN_OPERATOR;
        return pos + advance;
    }
    
    advance = parse_string(input, pos, token->value);
    if (advance > 0) {
        token->type = TOKEN_STRING;
        return pos + advance;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[pos];
    token->value[1] = '\0';
    return pos + 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;