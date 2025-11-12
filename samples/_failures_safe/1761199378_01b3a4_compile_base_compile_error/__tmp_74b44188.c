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
    while (**input && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = OPERATOR;
    return 1;
}

int parse_punctuation(const char **input, struct Token *token) {
    if (is_punctuation_char(**input)) {
        token->value[0] = **input;
        token->value[1] = '\0';
        token->type = PUNCTUATION;
        (*input)++;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token tokens[], int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        struct Token current_token;
        int parsed = 0;
        
        if (isdigit(*input)) {
            parsed = parse_number(&input, &current_token);
        } else if (isalpha(*input) || *input == '_') {
            parsed = parse_identifier(&input, &current_token);
        } else if (is_operator_char(*input)) {
            parsed = parse_operator(&input, &current_token);
        } else if (is_punctuation_char(*input)) {
            parsed = parse_punctuation(&input, &current_token);
        }
        
        if (parsed) {
            tokens[token_count++] = current_token;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    const char *names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
    if (type >= NUMBER && type <= END) {
        printf("%s", names[type]);
    }
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    int i = 0;
    while (i < token_count) {
        printf("  ");
        print_token_type(tokens[i].type);
        printf(": '%s'\n", tokens[i].value);
        i++;
    }
    
    int j = 0;
    printf("Token types: ");
    do {
        print_token_type(tokens[j].type);
        if (j < token_count - 1) printf(", ");
        j++;
    } while (j < token_count);
    printf("\n");
    
    printf("Processing tokens: ");
    for (int k = 0; k < token_count; k++) {
        if (tokens[k].type == END) break;
        printf("[%s