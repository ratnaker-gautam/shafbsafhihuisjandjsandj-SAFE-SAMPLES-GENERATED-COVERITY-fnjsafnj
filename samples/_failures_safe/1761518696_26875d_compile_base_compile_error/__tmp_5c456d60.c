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
        if (**input == '\n') {
            (*line)++;
        }
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isdigit(**input) || **input == '.')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    return i > 0;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    if (**input && is_operator_char(**input)) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && is_operator_char(**input) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char **input, struct Token *token, int *line) {
    int i = 0;
    if (**input == '"') {
        token->value[i++] = **input;
        (*input)++;
        while (**input && **input != '"') {
            if (**input == '\n') {
                (*line)++;
            }
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        if (**input == '"') {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
            token->value[i] = '\0';
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char **input, struct Token *token, int *line) {
    skip_whitespace(input, line);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, token)) {
        token->type = TOKEN_NUMBER;
        token->line = *line;
        return 1;
    }
    
    if (parse_string(input, token, line)) {
        token->type = TOKEN_STRING;
        token->line = *line;
        return 1;
    }
    
    if (parse_identifier(input, token)) {
        token->type = TOKEN_IDENTIFIER;
        token->line = *line;
        return 1;
    }
    
    if (parse_operator(input, token)) {
        token->type = TOKEN_OPERATOR;
        token->line = *line;
        return 1;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d: %s -> '%s'\n", token->line, type_names[token->type], token->value);
}

int main(void) {
    char input_buffer[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    int line = 1;
    
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input_buffer) - 1);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    const char *input_ptr = input_buffer;
    
    while (token_count < MAX_TOKENS) {
        struct Token token;
        if (!get_next_token(&input_ptr, &token, &line)) {
            if (*input_ptr) {
                fprintf(stderr, "Error: Unexpected character '%c' at line %d\n", *input_ptr, line);
                return 1;
            }
            break;
        }
        
        tokens[token_count] = token;
        token_count++;
        
        if (token.type