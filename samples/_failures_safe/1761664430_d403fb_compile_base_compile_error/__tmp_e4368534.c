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
    if (!input || !*input || !token) return 0;
    
    char *end = NULL;
    double num = strtod(*input, &end);
    if (end == *input) return 0;
    
    size_t len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (!input || !*input || !token || **input != '"') return 0;
    
    const char *start = *input + 1;
    const char *ptr = start;
    size_t len = 0;
    
    while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 1) {
        if (*ptr == '\\' && *(ptr + 1)) {
            ptr += 2;
            len += 2;
        } else {
            ptr++;
            len++;
        }
    }
    
    if (*ptr != '"') return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr + 1;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    if (!input || !*input || !token || (!isalpha(**input) && **input != '_')) return 0;
    
    const char *ptr = *input;
    size_t len = 0;
    
    while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
        ptr++;
        len++;
    }
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    if (!input || !*input || !token || !is_operator_char(**input)) return 0;
    
    const char *ptr = *input;
    size_t len = 0;
    
    while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
        ptr++;
        len++;
    }
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    
    while (isspace(**input)) (*input)++;
    
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

void tokenize(const char *input, Token *tokens, int *token_count) {
    if (!input || !tokens || !token_count) return;
    
    *token_count = 0;
    
    while (*input && *token_count < MAX_TOKENS - 1) {
        if (!get_next_token(&input, &tokens[*token_count])) {
            break;
        }
        (*token_count)++;
    }
    
    tokens[*token_count].type = TOKEN_EOF;
    tokens[*token_count].value[0] = '\0';
    (*token_count)++;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");