//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 31) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    
    int start = *pos;
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    
    int len = *pos - start;
    if (len > 31) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_symbol(const char *input, int *pos, struct Token *token) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int start = *pos;
    while (is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 31) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_SYMBOL;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_symbol(input, pos, token)) return 1;
    
    return 0;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_SYMBOL: printf("SYMBOL"); break;
        case TOKEN_EOF: printf("EOF"); break;
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
    
    int pos = 0;
    struct Token token;
    int token_count = 0;
    
    printf("Tokens:\n");
    
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        printf("Token %d: ", ++token_count);
        print_token_type(token.type);
        printf(" [%s]\n", token.value);
    }
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}