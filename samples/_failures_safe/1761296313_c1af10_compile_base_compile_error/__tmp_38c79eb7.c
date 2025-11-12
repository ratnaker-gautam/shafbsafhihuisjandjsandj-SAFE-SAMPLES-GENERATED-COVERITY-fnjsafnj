//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int i = 0;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        if (isdigit(c)) {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = c;
            }
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = c;
            }
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (i > 0) {
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    int i = 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_symbol(const char *input, int *pos, char *buffer) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int i = 0;
    while (input[*pos] != '\0' && is_symbol_char(input[*pos])) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (i > 0) {
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
        } else if (parse_symbol(input, &pos, token->value)) {
            token->type = TOKEN_SYMBOL;
            token_count++;
        } else {
            char c = input[pos];
            if (isalnum(c) || c == '_') {
                int i = 0;
                while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
                    if (i < MAX_TOKEN_LEN - 1) {
                        token->value[i++] = input[pos];
                    }
                    pos++;
                }
                token->value[i] = '\0';
                token->type = TOKEN_SYMBOL;
                token_count++;
            } else {
                pos++;
            }
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_SYMBOL: printf("SYMBOL"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
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
    
    int token_count = tokenize(input