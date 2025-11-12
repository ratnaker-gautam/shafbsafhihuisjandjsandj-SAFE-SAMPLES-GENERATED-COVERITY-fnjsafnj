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
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, Token *token) {
    char *end = NULL;
    double val = strtod(*input, &end);
    if (end == *input) return 0;
    
    int len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    if (!isalpha(**input) && **input != '_') return 0;
    
    (*input)++;
    while (isalnum(**input) || **input == '_') {
        (*input)++;
    }
    
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    if (!is_operator_char(**input)) return 0;
    
    (*input)++;
    if (is_operator_char(**input)) {
        (*input)++;
    }
    
    int len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, Token *token) {
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
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    while (isspace(**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, token)) return 1;
    if (parse_string(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    
    token->type = TOKEN_ERROR;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        if (!get_next_token(&input, tokens + count)) {
            return -1;
        }
        if (tokens[count].type == TOKEN_EOF) {
            break;
        }
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
    }
    
    return count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
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
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens,