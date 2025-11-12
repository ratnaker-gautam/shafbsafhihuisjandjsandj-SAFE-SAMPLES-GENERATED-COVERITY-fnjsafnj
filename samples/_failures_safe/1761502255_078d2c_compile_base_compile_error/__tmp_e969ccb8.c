//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

Token extract_string(char **input) {
    Token token = {TOKEN_STRING, ""};
    char *start = *input;
    (*input)++;
    
    while (**input && **input != '"') {
        if (**input == '\\' && *(*input + 1)) {
            (*input)++;
        }
        (*input)++;
    }
    
    if (**input == '"') {
        size_t len = *input - start - 1;
        if (len < MAX_TOKEN_LEN - 1) {
            strncpy(token.value, start + 1, len);
            token.value[len] = '\0';
        }
        (*input)++;
    } else {
        token.type = TOKEN_INVALID;
    }
    
    return token;
}

Token extract_number(char **input) {
    Token token = {TOKEN_NUMBER, ""};
    char *start = *input;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len < MAX_TOKEN_LEN - 1) {
        strncpy(token.value, start, len);
        token.value[len] = '\0';
    }
    
    return token;
}

Token extract_identifier(char **input) {
    Token token = {TOKEN_IDENTIFIER, ""};
    char *start = *input;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len < MAX_TOKEN_LEN - 1) {
        strncpy(token.value, start, len);
        token.value[len] = '\0';
    }
    
    return token;
}

Token extract_operator(char **input) {
    Token token = {TOKEN_OPERATOR, ""};
    char *start = *input;
    
    while (**input && is_operator_char(**input)) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len < MAX_TOKEN_LEN - 1) {
        strncpy(token.value, start, len);
        token.value[len] = '\0';
    }
    
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        Token token;
        
        if (*input == '"') {
            token = extract_string(&input);
        } else if (isdigit(*input)) {
            token = extract_number(&input);
        } else if (is_operator_char(*input)) {
            token = extract_operator(&input);
        } else if (isalpha(*input) || *input == '_') {
            token = extract_identifier(&input);
        } else {
            token.type = TOKEN_INVALID;
            token.value[0] = *input;
            token.value[1] = '\0';
            input++;
        }
        
        tokens[count++] = token;
    }
    
    tokens[count].type = TOKEN_EOF;
    tokens[count].value[0] = '\0';
    
    return count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "INVALID";
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
    
    if (len <= 1) {
        printf("No input provided\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s\t%s\n", token_type_name(tokens[i].type