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
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input, int *line) {
    while (**input != '\0') {
        if (**input == '\n') {
            (*line)++;
            (*input)++;
        } else if (isspace(**input)) {
            (*input)++;
        } else {
            break;
        }
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (isdigit(**input) && i < MAX_TOKEN_LEN - 1) {
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    while ((isalnum(**input) || **input == '_') && i < MAX_TOKEN_LEN - 1) {
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    while (is_operator_char(**input) && i < MAX_TOKEN_LEN - 1) {
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, struct Token *token, int *line) {
    int i = 0;
    (*input)++;
    while (**input != '\0' && **input != '"' && i < MAX_TOKEN_LEN - 1) {
        if (**input == '\n') {
            (*line)++;
        }
        token->value[i++] = **input;
        (*input)++;
    }
    if (**input == '"') {
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    const char *ptr = input;

    while (*ptr != '\0' && token_count < max_tokens) {
        skip_whitespace(&ptr, &line);
        if (*ptr == '\0') break;

        tokens[token_count].line = line;

        if (isdigit(*ptr)) {
            if (!parse_number(&ptr, &tokens[token_count])) return -1;
            token_count++;
        } else if (isalpha(*ptr) || *ptr == '_') {
            if (!parse_identifier(&ptr, &tokens[token_count])) return -1;
            token_count++;
        } else if (is_operator_char(*ptr)) {
            if (!parse_operator(&ptr, &tokens[token_count])) return -1;
            token_count++;
        } else if (*ptr == '"') {
            if (!parse_string(&ptr, &tokens[token_count], &line)) return -1;
            token_count++;
        } else {
            ptr++;
        }
    }

    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }

    return token_count;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d: %s", token->line, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter input to tokenize (max %d chars):\n", 1023);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        fprintf(stderr, "Error during tokenization\n");
        return 1;
    }

    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(&