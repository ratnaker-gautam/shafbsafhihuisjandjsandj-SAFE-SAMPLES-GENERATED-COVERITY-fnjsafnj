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
    while (**input && isdigit(**input)) {
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
        
        struct Token *current = &tokens[token_count];
        
        if (isdigit(*input)) {
            if (!parse_number(&input, current)) return -1;
        } else if (isalpha(*input) || *input == '_') {
            if (!parse_identifier(&input, current)) return -1;
        } else if (is_operator_char(*input)) {
            if (!parse_operator(&input, current)) return -1;
        } else if (*input == '"') {
            if (!parse_string(&input, current)) return -1;
        } else {
            return -1;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
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
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    
    int i = 0;
    while (i < token_count) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
        i++;
    }
    
    for (int j = 0; j < token_count; j++) {
        if (tokens[j].type == TOKEN_EOF) {
            break;
        }
    }
    
    int k = 0;
    do {
        if (k >= token_count) break;
        k++;
    } while (1);
    
    return 0;
}