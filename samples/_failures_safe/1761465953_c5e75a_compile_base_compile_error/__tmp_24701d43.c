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
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (isdigit(c)) {
        while (isdigit(input[pos + len])) {
            if (len < 31) {
                buffer[len] = input[pos + len];
                len++;
            } else {
                break;
            }
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (isalpha(c) || c == '_') {
        while (isalnum(input[pos + len]) || input[pos + len] == '_') {
            if (len < 31) {
                buffer[len] = input[pos + len];
                len++;
            } else {
                break;
            }
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] == '"') {
        buffer[len++] = '"';
        pos++;
        while (input[pos + len - 1] != '"' && input[pos + len - 1] != '\0') {
            if (len < 31) {
                buffer[len] = input[pos + len - 1];
                len++;
            } else {
                break;
            }
        }
        if (input[pos + len - 1] == '"') {
            buffer[len] = '\0';
            return len + 1;
        }
    }
    return 0;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    int start_pos = *pos;
    int token_len = 0;
    char buffer[32] = {0};
    
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
    
    token.line = line;
    token.column = *column;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    if ((token_len = parse_number(input, *pos, buffer)) > 0) {
        token.type = TOKEN_NUMBER;
        strncpy(token.value, buffer, 31);
        *pos += token_len;
        *column += token_len;
    } else if ((token_len = parse_identifier(input, *pos, buffer)) > 0) {
        token.type = TOKEN_IDENTIFIER;
        strncpy(token.value, buffer, 31);
        *pos += token_len;
        *column += token_len;
    } else if ((token_len = parse_operator(input, *pos, buffer)) > 0) {
        token.type = TOKEN_OPERATOR;
        strncpy(token.value, buffer, 31);
        *pos += token_len;
        *column += token_len;
    } else if ((token_len = parse_string(input, *pos, buffer)) > 0) {
        token.type = TOKEN_STRING;
        strncpy(token.value, buffer, 31);
        *pos += token_len;
        *column += token_len;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
        (*column)++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen