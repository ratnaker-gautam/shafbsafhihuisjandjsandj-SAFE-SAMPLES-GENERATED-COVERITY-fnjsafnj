//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    int input_len = strlen(input);
    
    while (*pos < input_len && isdigit(input[*pos])) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    buffer[buf_pos] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    int input_len = strlen(input);
    
    if (*pos >= input_len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    buffer[buf_pos] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    int input_len = strlen(input);
    
    if (*pos >= input_len || input[*pos] != '"') return 0;
    (*pos)++;
    
    while (*pos < input_len && input[*pos] != '"' && input[*pos] != '\0') {
        if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    if (*pos >= input_len || input[*pos] != '"') return 0;
    (*pos)++;
    
    buffer[buf_pos] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int buf_pos = 0;
    int input_len = strlen(input);
    
    if (*pos >= input_len || !is_operator_char(input[*pos])) return 0;
    
    buffer[buf_pos++] = input[(*pos)++];
    
    if (*pos < input_len && is_operator_char(input[*pos])) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
        buffer[buf_pos++] = input[(*pos)++];
    }
    
    buffer[buf_pos] = '\0';
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int input_len = strlen(input);
    
    while (pos < input_len && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
            continue;
        }
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            token_count++;
        } else {
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
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