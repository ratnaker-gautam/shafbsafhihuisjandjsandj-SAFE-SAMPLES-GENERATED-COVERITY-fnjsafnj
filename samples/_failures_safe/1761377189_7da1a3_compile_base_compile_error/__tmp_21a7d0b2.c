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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    int has_dot = 0;
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_string(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    int i = 0;
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) {
            (*pos)++;
            if (input[*pos] == 'n') buffer[i++] = '\n';
            else if (input[*pos] == 't') buffer[i++] = '\t';
            else buffer[i++] = input[*pos];
        } else {
            if (i < 31) buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || !isalpha(input[*pos])) return 0;
    int i = 0;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (i < 31) buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_operator(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    int i = 0;
    while (*pos < len && is_operator_char(input[*pos]) && i < 31) {
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int next_token(const char *input, int len, int *pos, int *line, struct Token *token) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') (*line)++;
        (*pos)++;
    }
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        return 1;
    }
    token->line = *line;
    if (parse_number(input, pos, len, token->value)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    if (parse_string(input, pos, len, token->value)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    if (parse_identifier(input, pos, len, token->value)) {
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    if (parse_operator(input, pos, len, token->value)) {
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[--len] = '\0';
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    int pos = 0;
    int line = 1;
    struct Token token;
    printf("Tokens:\n");
    while (next_token(input, len, &pos, &line, &token)) {
        if (token.type == TOKEN_EOF) break;
        const char *type_str;
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Line %d: %