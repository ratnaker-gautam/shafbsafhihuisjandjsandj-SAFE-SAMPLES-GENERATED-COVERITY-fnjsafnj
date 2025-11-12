//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int has_dot = 0;
    int len = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            if (len < 63) buffer[len++] = input[*pos];
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            if (len < 63) buffer[len++] = input[*pos];
            (*pos)++;
            has_dot = 1;
        } else {
            break;
        }
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int len = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos)++;
            if (len < 63) buffer[len++] = input[*pos];
            (*pos)++;
        } else {
            if (len < 63) buffer[len++] = input[*pos];
            (*pos)++;
        }
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_symbol(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    while (input[*pos] != '\0' && is_symbol_char(input[*pos])) {
        if (len < 63) buffer[len++] = input[*pos];
        (*pos)++;
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token->value)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (parse_string(input, pos, token->value)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    
    if (parse_symbol(input, pos, token->value)) {
        token->type = TOKEN_SYMBOL;
        return 1;
    }
    
    return 0;
}

void print_token(struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"\n", token->value);
            break;
        case TOKEN_SYMBOL:
            printf("SYMBOL: %s\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    int token_count = 0;
    
    printf("Tokens:\n");
    while (get_next_token(input, &pos, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        print_token(&token);
        token_count++;
        
        if (token_count > 1000) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    
    if (token.type != TOKEN_EOF) {
        fprintf(stderr, "Invalid token at position %d\n", pos);
        return 1;
    }
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}