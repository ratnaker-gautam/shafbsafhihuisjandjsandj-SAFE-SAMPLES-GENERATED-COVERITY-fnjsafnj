//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_IDENTIFIER, TOKEN_EOF, TOKEN_UNKNOWN };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    int len = *pos - start;
    if (len > 31) return 0;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    (*pos)++;
    while (isalnum(input[*pos]) || input[*pos] == '_') (*pos)++;
    int len = *pos - start;
    if (len > 31) return 0;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *column = 1;
            } else (*column)++;
            (*pos)++;
            continue;
        }
        token->line = *line;
        token->column = *column;
        if (isdigit(input[*pos])) {
            if (parse_number(input, pos, token)) {
                *column += (int)strlen(token->value);
                return 1;
            } else return 0;
        } else if (is_operator(input[*pos])) {
            token->value[0] = input[*pos];
            token->value[1] = '\0';
            token->type = TOKEN_OPERATOR;
            (*pos)++;
            (*column)++;
            return 1;
        } else if (parse_identifier(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        } else {
            token->value[0] = input[*pos];
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            (*pos)++;
            (*column)++;
            return 1;
        }
    }
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 1;
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    printf("Tokens:\n");
    while (get_next_token(input, &pos, &line, &column, &token)) {
        if (token.type == TOKEN_EOF) break;
        const char *type_str;
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_UNKNOWN: type_str = "UNKNOWN"; break;
            default: type_str = "INVALID"; break;
        }
        printf("Line %d, Column %d: %s '%s'\n", token.line, token.column, type_str, token.value);
        if (token.type == TOKEN_UNKNOWN) {
            fprintf(stderr, "Invalid token encountered\n");
            return 1;
        }
    }
    return 0;
}