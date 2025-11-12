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
    int start = *pos;
    while (*pos < len && isdigit(input[*pos])) {
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return *pos > start;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    int start = *pos;
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 31) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        while (*pos < len && input[*pos] != '"') {
            if (i < 31) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
            if (*pos >= len) {
                return 0;
            }
        }
        if (*pos < len && input[*pos] == '"') {
            (*pos)++;
            buffer[i] = '\0';
            return 1;
        }
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    int start = *pos;
    while (*pos < len && is_operator_char(input[*pos])) {
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return *pos > start;
}

void skip_whitespace(const char *input, int *pos, int len, int *line, int *column) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
}

int get_next_token(const char *input, int len, int *pos, int *line, int *column, struct Token *token) {
    skip_whitespace(input, pos, len, line, column);
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 1;
    }
    token->line = *line;
    token->column = *column;
    if (parse_number(input, pos, len, token->value)) {
        token->type = TOKEN_NUMBER;
        *column += (int)strlen(token->value);
        return 1;
    }
    if (parse_identifier(input, pos, len, token->value)) {
        token->type = TOKEN_IDENTIFIER;
        *column += (int)strlen(token->value);
        return 1;
    }
    if (parse_string(input, pos, len, token->value)) {
        token->type = TOKEN_STRING;
        *column += (int)strlen(token->value) + 2;
        return 1;
    }
    if (parse_operator(input, pos, len, token->value)) {
        token->type = TOKEN_OPERATOR;
        *column += (int)strlen(token->value);
        return 1;
    }
    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    int len = (int)strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1