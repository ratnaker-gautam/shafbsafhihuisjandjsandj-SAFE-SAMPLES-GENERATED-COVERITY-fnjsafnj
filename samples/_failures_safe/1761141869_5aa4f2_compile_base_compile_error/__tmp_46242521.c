//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    int has_dot = 0;
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (i >= 31) return 0;
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    int i = 0;
    while (*pos < len && input[*pos] != '"') {
        if (i >= 31) return 0;
        if (input[*pos] == '\\') {
            (*pos)++;
            if (*pos >= len) return 0;
        }
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    buffer[i] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || !isalpha(input[*pos])) return 0;
    int i = 0;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (i >= 31) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    int i = 0;
    while (*pos < len && is_operator_char(input[*pos]) && i < 2) {
        if (i >= 31) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int get_next_token(const char *input, int len, int *pos, int *line, int *column, struct Token *token) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 1;
    }
    token->line = *line;
    token->column = *column;
    char temp[32];
    if (parse_string(input, pos, len, temp)) {
        token->type = TOKEN_STRING;
        strncpy(token->value, temp, 31);
        token->value[31] = '\0';
        *column += (int)strlen(temp) + 2;
        return 1;
    }
    if (parse_number(input, pos, len, temp)) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, temp, 31);
        token->value[31] = '\0';
        *column += (int)strlen(temp);
        return 1;
    }
    if (parse_identifier(input, pos, len, temp)) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, temp, 31);
        token->value[31] = '\0';
        *column += (int)strlen(temp);
        return 1;
    }
    if (parse_operator(input, pos, len, temp)) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, temp, 31);
        token->value[31] = '\0';
        *column += (int)strlen(temp);
        return 1;
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = { "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF" };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" [%s]", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter input to tokenize: