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
    TOKEN_EOF,
    TOKEN_INVALID
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

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit(**input) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_') && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (**input == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        int i = 0;
        while (**input && **input != '"' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        if (**input == '"') {
            (*input)++;
        }
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int tokenize_input(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    const char *current = input;
    
    while (*current && count < max_tokens) {
        struct Token token = get_next_token(&current);
        tokens[count] = token;
        count++;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return count;
}

int main(void) {
    char input_buffer[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize_input(input_buffer, tokens, MAX_TOKENS);
    
    printf("\nTokenized output:\n");
    printf("%-12s %s\n", "TYPE", "VALUE");
    printf("------------ ---------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s '%s'\n", token_type_to_string(tokens[i].type), tokens[i].value);
        if (tokens[i].type == TOKEN_EOF) {
            break;
        }
    }
    
    return 0;
}