//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isdigit(**input) || **input == '.')) {
        if (i >= 31) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, struct Token *token) {
    if (**input != '"') return 0;
    (*input)++;
    int i = 0;
    while (**input && **input != '"') {
        if (i >= 31) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    if (**input != '"') return 0;
    (*input)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    if (!isalpha(**input) && **input != '_') return 0;
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= 31) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    if (!is_operator_char(**input)) return 0;
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i >= 31) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char **input, struct Token *token) {
    skip_whitespace(input);
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    if (parse_number(input, token)) return 1;
    if (parse_string(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "STRING", "IDENTIFIER", "OPERATOR", "EOF"
    };
    printf("Token: %-12s Value: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input_buffer[1024];
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    const char *input_ptr = input_buffer;
    struct Token token;
    int token_count = 0;
    while (get_next_token(&input_ptr, &token)) {
        if (token.type == TOKEN_EOF) break;
        print_token(&token);
        token_count++;
        if (token_count >= 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    if (*input_ptr) {
        fprintf(stderr, "Unexpected character: %c\n", *input_ptr);
        return 1;
    }
    printf("Total tokens: %d\n", token_count);
    return 0;
}