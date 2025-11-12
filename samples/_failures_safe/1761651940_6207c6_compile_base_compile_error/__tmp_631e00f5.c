//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, PUNCTUATION, END };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    const char *start = *input;
    int has_digit = 0;
    int dot_count = 0;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        if (**input == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else {
            has_digit = 1;
        }
        (*input)++;
    }
    
    if (!has_digit) return 0;
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    const char *start = *input;
    
    if (!isalpha(**input) && **input != '_') return 0;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    if (!is_operator_char(**input)) return 0;
    
    token->value[0] = **input;
    token->value[1] = '\0';
    token->type = OPERATOR;
    (*input)++;
    return 1;
}

int parse_punctuation(const char **input, struct Token *token) {
    if (!is_punctuation_char(**input)) return 0;
    
    token->value[0] = **input;
    token->value[1] = '\0';
    token->type = PUNCTUATION;
    (*input)++;
    return 1;
}

int tokenize(const char *input, struct Token tokens[], int max_tokens) {
    int token_count = 0;
    const char *original_input = input;
    
    if (!input || !tokens || max_tokens <= 0) {
        return 0;
    }
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        struct Token token;
        int parsed = 0;
        
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        else if (parse_punctuation(&input, &token)) parsed = 1;
        
        if (parsed) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
            }
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case NUMBER: printf("NUMBER"); break;
        case IDENTIFIER: printf("IDENTIFIER"); break;
        case OPERATOR: printf("OPERATOR"); break;
        case PUNCTUATION: printf("PUNCTUATION"); break;
        case END: printf("END"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
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
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("Tokens found: %d\n",