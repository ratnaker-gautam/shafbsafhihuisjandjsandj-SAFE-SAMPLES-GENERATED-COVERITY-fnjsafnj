//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int len = 0;
    while (isdigit(input[*pos])) {
        if (len >= 63) return 0;
        buffer[len++] = input[(*pos)++];
    }
    buffer[len] = '\0';
    return len > 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int len = 0;
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (len >= 63) return 0;
        buffer[len++] = input[(*pos)++];
    }
    if (input[*pos] != '"') return 0;
    (*pos)++;
    buffer[len] = '\0';
    return 1;
}

int parse_symbol(const char *input, int *pos, char *buffer) {
    if (!is_symbol_char(input[*pos])) return 0;
    int len = 0;
    while (is_symbol_char(input[*pos])) {
        if (len >= 63) return 0;
        buffer[len++] = input[(*pos)++];
    }
    buffer[len] = '\0';
    return len > 0;
}

int skip_whitespace(const char *input, int *pos) {
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') return 1;
        (*pos)++;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int *line, struct Token *token) {
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        return 1;
    }

    if (skip_whitespace(input, pos)) {
        (*line)++;
    }

    if (parse_number(input, pos, token->value)) {
        token->type = TOKEN_NUMBER;
        token->line = *line;
        return 1;
    }

    if (parse_string(input, pos, token->value)) {
        token->type = TOKEN_STRING;
        token->line = *line;
        return 1;
    }

    if (parse_symbol(input, pos, token->value)) {
        token->type = TOKEN_SYMBOL;
        token->line = *line;
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

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    int pos = 0;
    int line = 1;
    struct Token token;
    int token_count = 0;

    printf("Tokens:\n");
    while (get_next_token(input, &pos, &line, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }

        const char *type_str;
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_SYMBOL: type_str = "SYMBOL"; break;
            default: type_str = "UNKNOWN"; break;
        }

        printf("Line %d: %s '%s'\n", token.line, type_str, token.value);
        token_count++;

        if (token_count > 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }

    if (input[pos] != '\0') {
        fprintf(stderr, "Unexpected character at position %d: '%c'\n", pos, input[pos]);
        return 1;
    }

    printf("Total tokens: %d\n", token_count);
    return 0;
}