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
    if (**input != '"') return 0;
    
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
    if (!isalpha(**input) && **input != '_') return 0;
    
    const char *start = *input;
    const char *ptr = start;
    size_t len = 0;
    
    while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
        ptr++;
        len++;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    if (!is_operator_char(**input)) return 0;
    
    const char *start = *input;
    const char *ptr = start;
    size_t len = 0;
    
    while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
        ptr++;
        len++;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        while (isspace(*input)) input++;
        if (!*input) break;
        
        Token token = {TOKEN_ERROR, ""};
        
        if (parse_number(&input, &token) ||
            parse_string(&input, &token) ||
            parse_identifier(&input, &token) ||
            parse_operator(&input, &token)) {
            tokens[count++] = token;
        } else {
            tokens[count].type = TOKEN_ERROR;
            tokens[count].value[0] = *input;
            tokens[count].value[1] = '\0';
            count++;
            input++;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
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
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(t