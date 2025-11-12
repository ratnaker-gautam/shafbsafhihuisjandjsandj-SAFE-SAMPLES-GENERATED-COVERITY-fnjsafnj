//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, Token *token) {
    int start = *pos;
    while (isdigit(input[*pos])) {
        (*pos)++;
    }
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, Token *token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int start = *pos;
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    if (input[*pos] != '"') return 0;
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_operator(const char *input, int *pos, Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    int start = *pos;
    while (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_identifier(const char *input, int *pos, Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        
        Token token;
        int success = 0;
        
        if (parse_number(input, &pos, &token)) success = 1;
        else if (parse_string(input, &pos, &token)) success = 1;
        else if (parse_operator(input, &pos, &token)) success = 1;
        else if (parse_identifier(input, &pos, &token)) success = 1;
        
        if (success) {
            tokens[token_count] = token;
            token_count++;
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
        if (tokens[i].type == TOKEN_EOF) break;
    }
}

int main() {
    char input[4096];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    print_tokens(tokens, token