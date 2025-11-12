//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    char c = input[pos];
    if (c == '\0') return 0;
    
    if (is_operator_char(c)) {
        output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    while (input[pos] != '\0' && len < 31) {
        char c = input[pos];
        if (isdigit(c)) {
            output[len++] = c;
            pos++;
        } else if (c == '.' && !has_dot) {
            output[len++] = c;
            has_dot = 1;
            pos++;
        } else {
            break;
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
        while (input[pos] != '\0' && input[pos] != '"' && len < 31) {
            output[len++] = input[pos];
            pos++;
        }
        if (input[pos] == '"') {
            pos++;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len + 2 : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if ((isalpha(input[pos]) || input[pos] == '_') && input[pos] != '\0') {
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_') && len < 31) {
            output[len++] = input[pos];
            pos++;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    if (input == NULL || pos == NULL || column == NULL || *pos < 0) {
        return token;
    }
    
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
        
        int advance = 0;
        
        if (is_operator_char(c)) {
            advance = parse_operator(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_OPERATOR;
            }
        } else if (isdigit(c)) {
            advance = parse_number(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_NUMBER;
            }
        } else if (c == '"') {
            advance = parse_string(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_STRING;
            }
        } else if (isalpha(c) || c == '_') {
            advance = parse_identifier(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_IDENTIFIER;
            }
        }
        
        if (advance > 0) {
            *pos += advance;
            *column += advance;
            token.line = line;
            token.column = *column - advance;
            return token;
        } else {
            if (strlen(token.value) < 31) {
                token.value[0] = c;
                token.value[1] = '\0';
            }
            (*pos)++;
            (*column)++;
            token.line = line;
            token.column = *column - 1;
            return token