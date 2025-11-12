//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    if (input == NULL || *input == NULL) return;
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int parse_number(char **input, struct Token *token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    char *start = *input;
    char *ptr = *input;
    
    if (*ptr == '-' || *ptr == '+') {
        ptr++;
    }
    
    if (!isdigit((unsigned char)*ptr)) {
        return 0;
    }
    
    while (*ptr && isdigit((unsigned char)*ptr)) {
        ptr++;
    }
    
    if (*ptr == '.') {
        ptr++;
        while (*ptr && isdigit((unsigned char)*ptr)) {
            ptr++;
        }
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_word(char **input, struct Token *token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    char *start = *input;
    char *ptr = *input;
    
    if (!isalpha((unsigned char)*ptr) && *ptr != '_') {
        return 0;
    }
    
    ptr++;
    while (*ptr && (isalnum((unsigned char)*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_WORD;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, struct Token *token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    char *start = *input;
    char *ptr = *input;
    
    if (!is_operator_char(*ptr)) {
        return 0;
    }
    
    ptr++;
    if (is_operator_char(*ptr)) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int get_next_token(char **input, struct Token *token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    skip_whitespace(input);
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, token)) return 1;
    if (parse_word(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    
    return 0;
}

int tokenize(char *input, struct Token *tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int count = 0;
    char *ptr = input;
    
    while (*ptr && count < max_tokens) {
        struct Token token;
        if (!get_next_token(&ptr, &token)) {
            break;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        tokens[count] = token;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
    }
    
    return count;
}

void print_tokens(struct Token *tokens, int count) {
    if (tokens == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        const char *type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_WORD: type_str = "WORD"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %d: %s -> '%s'\n", i, type_str, tokens[i].value);
    }
}

int main(void) {
    char input[1024