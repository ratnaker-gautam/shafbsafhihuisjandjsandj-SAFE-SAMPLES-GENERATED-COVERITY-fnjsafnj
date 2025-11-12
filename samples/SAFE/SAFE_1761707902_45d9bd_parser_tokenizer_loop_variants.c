//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
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
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, struct Token *token) {
    if (**input != '"') return 0;
    (*input)++;
    int i = 0;
    while (**input && **input != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    if (**input != '"') return 0;
    (*input)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;

        struct Token token;
        int success = 0;

        if (isdigit(*input)) {
            success = parse_number(&input, &token);
        } else if (isalpha(*input) || *input == '_') {
            success = parse_identifier(&input, &token);
        } else if (is_operator_char(*input)) {
            success = parse_operator(&input, &token);
        } else if (*input == '"') {
            success = parse_string(&input, &token);
        } else {
            break;
        }

        if (success) {
            tokens[token_count++] = token;
        } else {
            break;
        }
    }

    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    return token_count;
}

void print_tokens(const struct Token *tokens, int count) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };

    int i = 0;
    do {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
        i++;
    } while (i <= count && tokens[i-1].type != TOKEN_EOF);
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

    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);

    if (token_count == 0 && *input) {
        fprintf(stderr, "Failed to tokenize input\n");
        return 1;
    }

    print_tokens(tokens, token_count);
    return 0;
}