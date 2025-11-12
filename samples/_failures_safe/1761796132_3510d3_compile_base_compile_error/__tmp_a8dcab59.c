//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    int i = 0;
    while (**input && isdigit(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    if (!isalpha(**input) && **input != '_') return 0;
    
    token->value[i++] = **input;
    (*input)++;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    if (!is_operator_char(**input)) return 0;
    
    token->value[i++] = **input;
    (*input)++;
    
    if (is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = OPERATOR;
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

const char* token_type_str(enum TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case OPERATOR: return "OPERATOR";
        case PUNCTUATION: return "PUNCTUATION";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct Token tokens[], int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        struct Token *current = &tokens[token_count];
        
        int parsed = 0;
        parsed = parse_number(&ptr, current);
        if (!parsed) parsed = parse_identifier(&ptr, current);
        if (!parsed) parsed = parse_operator(&ptr, current);
        if (!parsed) parsed = parse_punctuation(&ptr, current);
        
        if (!parsed) {
            fprintf(stderr, "Error: Invalid character '%c'\n", *ptr);
            return -1;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
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
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    int i = 0;
    while (i < token_count) {
        if (tokens[i].type == END) break;
        printf("%-12s\t%s\n", token_type_str(tokens[i].type), tokens[i].value);
        i++;
    }
    
    for (int j = 0; j < token_count; j++) {
        if (tokens[j].type == END