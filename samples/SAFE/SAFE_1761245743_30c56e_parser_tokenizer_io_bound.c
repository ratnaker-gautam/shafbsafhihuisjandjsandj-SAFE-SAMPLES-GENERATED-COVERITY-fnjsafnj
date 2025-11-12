//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    while (**input && isspace(**input)) {
        if (**input == '\n') (*line)++;
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isdigit(**input) || **input == '.')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = *(*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = *(*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = *(*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, struct Token *token, int *line) {
    int i = 0;
    (*input)++;
    while (**input && **input != '"') {
        if (**input == '\n') (*line)++;
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = *(*input)++;
    }
    if (**input != '"') return 0;
    (*input)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int line = 1;
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input, &line);
        if (!*input) break;
        
        tokens[token_count].line = line;
        
        if (isdigit(*input)) {
            if (!parse_number(&input, &tokens[token_count])) return -1;
        } else if (isalpha(*input) || *input == '_') {
            if (!parse_identifier(&input, &tokens[token_count])) return -1;
        } else if (is_operator_char(*input)) {
            if (!parse_operator(&input, &tokens[token_count])) return -1;
        } else if (*input == '"') {
            if (!parse_string(&input, &tokens[token_count], &line)) return -1;
        } else {
            return -1;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].line = line;
        strcpy(tokens[token_count].value, "");
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
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count <= 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(&tokens[i]);
    }
    
    return 0;
}