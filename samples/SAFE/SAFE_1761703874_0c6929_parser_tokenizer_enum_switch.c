//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_next_token(const char *input, int *pos, int line, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return -1;
    
    int start = *pos;
    int len = strlen(input);
    
    while (start < len && isspace(input[start])) {
        if (input[start] == '\n') line++;
        start++;
    }
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        *pos = start;
        return 0;
    }
    
    token->line = line;
    
    if (isdigit(input[start])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (start < len && i < 31 && isdigit(input[start])) {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
    } else if (isalpha(input[start]) || input[start] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (start < len && i < 31 && (isalnum(input[start]) || input[start] == '_')) {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
    } else if (input[start] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        start++;
        while (start < len && i < 31 && input[start] != '"') {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
        if (start < len && input[start] == '"') start++;
    } else if (is_operator_char(input[start])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (start < len && i < 31 && is_operator_char(input[start])) {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = input[start++];
        token->value[1] = '\0';
    }
    
    *pos = start;
    return 0;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    const char *type_str;
    switch (token->type) {
        case TOKEN_NUMBER:
            type_str = "NUMBER";
            break;
        case TOKEN_IDENTIFIER:
            type_str = "IDENTIFIER";
            break;
        case TOKEN_OPERATOR:
            type_str = "OPERATOR";
            break;
        case TOKEN_STRING:
            type_str = "STRING";
            break;
        case TOKEN_EOF:
            type_str = "EOF";
            break;
        case TOKEN_UNKNOWN:
            type_str = "UNKNOWN";
            break;
        default:
            type_str = "INVALID";
            break;
    }
    
    printf("Line %d: %s", token->line, type_str);
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    struct Token token;
    
    printf("\nTokens:\n");
    while (1) {
        if (get_next_token(input, &pos, line, &token) != 0) {
            fprintf(stderr, "Error tokenizing input\n");
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        line = token.line;
    }
    
    return 0;
}