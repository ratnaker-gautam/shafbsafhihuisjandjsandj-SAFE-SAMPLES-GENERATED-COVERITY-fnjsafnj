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
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return i > 0;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return i > 0;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return i > 0;
}

int parse_string(const char **input, struct Token *token) {
    if (**input != '"') return 0;
    (*input)++;
    
    int i = 0;
    while (**input && **input != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    
    if (**input == '"') {
        (*input)++;
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        struct Token token;
        int parsed = 0;
        
        if (isdigit(*input)) {
            parsed = parse_number(&input, &token);
        } else if (isalpha(*input) || *input == '_') {
            parsed = parse_identifier(&input, &token);
        } else if (is_operator_char(*input)) {
            parsed = parse_operator(&input, &token);
        } else if (*input == '"') {
            parsed = parse_string(&input, &token);
        } else {
            input++;
            continue;
        }
        
        if (parsed) {
            tokens[token_count++] = token;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
    return token_count + 1;
}

void print_token_type(enum TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("%s", names[type]);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS - 1);
    
    printf("Tokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}