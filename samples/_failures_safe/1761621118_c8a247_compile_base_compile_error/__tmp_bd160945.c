//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LENGTH];
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LENGTH - 1) break;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[len++] = input[(*pos)++];
        
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (len >= MAX_TOKEN_LENGTH - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (is_operator_char(input[*pos])) {
        buffer[len++] = input[(*pos)++];
        
        if (is_operator_char(input[*pos]) && len < MAX_TOKEN_LENGTH - 1) {
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (input[*pos] == '"') {
        buffer[len++] = input[(*pos)++];
        
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LENGTH - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        if (input[*pos] == '"') {
            buffer[len++] = input[(*pos)++];
            buffer[len] = '\0';
            return 1;
        }
    }
    
    return 0;
}

void skip_whitespace(const char *input, int *pos) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        
        if (input[pos] == '\0') break;
        
        if (parse_number(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (parse_identifier(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        if (parse_operator(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
            continue;
        }
        
        if (parse_string(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
            continue;
        }
        
        break;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "EOF", MAX_TOKEN_LENGTH - 1);
        tokens[token_count].value[MAX_TOKEN_LENGTH - 1] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char *token_type_to_string(enum TokenType type) {
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
    char input[MAX_INPUT_LENGTH];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n