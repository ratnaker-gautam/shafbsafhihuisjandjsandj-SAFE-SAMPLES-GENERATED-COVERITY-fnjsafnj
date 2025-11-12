//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    int length = *pos - start;
    if (length > 31) return 0;
    memcpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int start = *pos;
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) (*pos)++;
        (*pos)++;
    }
    if (*pos >= len) return 0;
    int length = *pos - start;
    if (length > 31) return 0;
    memcpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    int start = *pos;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    int length = *pos - start;
    if (length > 31) return 0;
    memcpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    int start = *pos;
    (*pos)++;
    if (*pos < len && is_operator_char(input[*pos]) && input[start] != '!' && input[*pos] == '=') {
        (*pos)++;
    }
    int length = *pos - start;
    if (length > 31) return 0;
    memcpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char *input, int len, int *pos, int *line, int *column, struct Token *token) {
    while (*pos < len) {
        char c = input[*pos];
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        token->line = *line;
        token->column = *column;
        if (parse_number(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        if (parse_string(input, pos, len, token)) {
            *column += (int)strlen(token->value) + 2;
            return 1;
        }
        if (parse_identifier(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        if (parse_operator(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        return 0;
    }
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    int len = (int)strlen(input);
    if (len >