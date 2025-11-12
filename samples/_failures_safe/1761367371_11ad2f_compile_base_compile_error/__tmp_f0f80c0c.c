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

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
        output[len] = '\0';
    }
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] && (isdigit(input[pos]) || input[pos] == '.')) {
        if (input[pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (len < 31) {
            output[len++] = input[pos++];
        } else {
            break;
        }
    }
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] && (isalpha(input[pos]) || input[pos] == '_')) {
        while (input[pos] && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 31) {
                output[len++] = input[pos++];
            } else {
                break;
            }
        }
    }
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        pos++;
        while (input[pos] && input[pos] != '"') {
            if (len < 31) {
                output[len++] = input[pos++];
            } else {
                break;
            }
        }
        if (input[pos] == '"') {
            pos++;
        }
    }
    output[len] = '\0';
    return len + (len > 0 ? 2 : 0);
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    int start_pos = *pos;
    
    while (input[*pos] && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (!input[*pos]) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    token.line = line;
    token.column = *column;
    
    int advance = 0;
    
    if (isdigit(input[*pos])) {
        advance = parse_number(input, *pos, token.value);
        if (advance > 0) {
            token.type = TOKEN_NUMBER;
        }
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        advance = parse_identifier(input, *pos, token.value);
        if (advance > 0) {
            token.type = TOKEN_IDENTIFIER;
        }
    } else if (input[*pos] == '"') {
        advance = parse_string(input, *pos, token.value);
        if (advance > 2) {
            token.type = TOKEN_STRING;
        }
    } else if (is_operator_char(input[*pos])) {
        advance = parse_operator(input, *pos, token.value);
        if (advance > 0) {
            token.type = TOKEN_OPERATOR;
        }
    }
    
    if (advance == 0) {
        if (token.value[0] == '\0' && *pos - start_pos < 31) {
            strncpy(token.value, &input[*pos], 1);
            token.value[1] = '\0';
        }
        (*pos)++;
        (*column)++;
    } else {
        *pos += advance;
        *column += advance;
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
    if (len > 0 && input[len - 1] == '\n