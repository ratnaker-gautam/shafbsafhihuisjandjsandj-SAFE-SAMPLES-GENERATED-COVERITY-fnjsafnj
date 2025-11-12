//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    char *end;
    long num = strtol(*input, &end, 10);
    if (end == *input) return 0;
    
    if (num > 2147483647L || num < -2147483648L) return 0;
    
    int len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    const char *start = *input;
    if (!isalpha(**input) && **input != '_') return 0;
    
    (*input)++;
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    if (!is_operator_char(**input)) return 0;
    
    const char *start = *input;
    (*input)++;
    
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, struct Token *token) {
    if (**input != '"') return 0;
    
    const char *start = *input;
    (*input)++;
    
    while (**input && **input != '"') {
        if (**input == '\\' && *(*input + 1)) {
            (*input) += 2;
        } else {
            (*input)++;
        }
    }
    
    if (**input != '"') return 0;
    
    (*input)++;
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        skip_whitespace(&input);
        if (!*input) break;
        
        struct Token token;
        int parsed = 0;
        
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        
        if (parsed) {
            tokens[count] = token;
            count++;
        } else {
            break;
        }
    }
    
    tokens[count].type = TOKEN_EOF;
    tokens[count].value[0] = '\0';
    return count + 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return