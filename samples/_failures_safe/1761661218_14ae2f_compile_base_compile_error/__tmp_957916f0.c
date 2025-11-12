//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

void tokenize(const char* input) {
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        while (isspace((unsigned char)*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (isdigit((unsigned char)*ptr)) {
            size_t len = 0;
            while (isdigit((unsigned char)*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            size_t len = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (*ptr == '"') {
            ptr++;
            size_t len = 0;
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
}

Token get_next_token() {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token eof_token = {TOKEN_EOF, ""};
    return eof_token;
}

void reset_parser() {
    current_token = 0;
}

void print_token(Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Type: %-12s Value: %s\n", type_names[token.type], token.value);
}

int parse_expression();

int parse_factor() {
    Token token = get_next_token();
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        return 1;
    }
    else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        int result = parse_expression();
        token = get_next_token();
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            return 0;
        }
        return result;
    }
    return 0;
}

int parse_term() {
    if (!parse_factor()) return 0;
    
    Token token = get_next_token();
    while (token.type == TOKEN_OPERATOR && 
          (token.value[0] == '*' || token.value[0] == '/')) {
        if (!parse_factor()) return 0;
        token = get_next_token();
    }
    if (current_token > 0) current_token--;
    return 1;
}

int parse_expression() {
    if (!parse_term()) return 0;
    
    Token token = get_next_token();
    while (token.type == TOKEN_OPERATOR && 
          (token.value[0] == '+' || token.value[0] == '-')) {
        if (!parse_term()) return 0;
        token = get_next_token();
    }
    if (current_token > 0) current_token--;
    return 1;
}

int parse_assignment() {
    Token token = get_next_token();
    if (token.type != TOKEN_IDENTIFIER) return 0;
    
    token = get_next_token();
    if (token.type != TOKEN_OPERATOR || token.value[0] != '=') return 0;
    
    return parse_expression();
}

int main() {
    char input[1024];
    
    printf("Enter expressions to parse (empty line to exit):\n");
    
    while (1) {
        printf