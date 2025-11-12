//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
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
    while (isspace(input[*pos])) (*pos)++;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!skip_whitespace(input, pos)) return 0;
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    if (parse_number(input, pos, token->value)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    if (parse_string(input, pos, token->value)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    if (parse_symbol(input, pos, token->value)) {
        token->type = TOKEN_SYMBOL;
        return 1;
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Token: %-8s Value: '%s'\n", type_names[token->type], token->value);
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
    struct Token token;
    int token_count = 0;
    while (get_next_token(input, &pos, &token)) {
        if (token.type == TOKEN_EOF) break;
        print_token(&token);
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