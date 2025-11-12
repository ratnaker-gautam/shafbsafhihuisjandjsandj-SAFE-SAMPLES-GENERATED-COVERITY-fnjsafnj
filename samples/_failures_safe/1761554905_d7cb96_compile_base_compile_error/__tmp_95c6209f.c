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

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case OPERATOR: return "OPERATOR";
        case PUNCTUATION: return "PUNCTUATION";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        struct Token *current = &tokens[token_count];
        int parsed = 0;
        
        if (parse_number(&input, current)) parsed = 1;
        else if (parse_identifier(&input, current)) parsed = 1;
        else if (parse_operator(&input, current)) parsed = 1;
        else if (parse_punctuation(&input, current)) parsed = 1;
        
        if (parsed) {
            token_count++;
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

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    int i = 0;
    while (i < token_count) {
        printf("%s\t'%s'\n", token_type_name(tokens[i].type), tokens[i].value);
        i++;
    }
    
    int j = 0;
    do {
        if (tokens[j].type == END) break;
        j++;
    } while (j < token_count);
    
    for (int k = 0; k < token_count; k++) {
        if (tokens[k].type == END) {
            printf("Reached