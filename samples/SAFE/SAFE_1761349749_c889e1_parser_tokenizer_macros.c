//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        size_t len = 0;
        
        if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (*p == '"') {
            token.type = TOKEN_STRING;
            p++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            if (*p == '"') p++;
        } else if (is_operator(*p)) {
            token.type = TOKEN_OPERATOR;
            token.value[len++] = *p++;
        } else {
            token.type = TOKEN_UNKNOWN;
            token.value[len++] = *p++;
        }
        
        token.value[len] = '\0';
        if (len > 0) {
            tokens[token_count++] = token;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int parse_expression(void) {
    if (current_token >= token_count) return 0;
    
    Token token = tokens[current_token];
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        current_token++;
        if (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR) {
            current_token++;
            return parse_expression();
        }
        return 1;
    }
    
    return 0;
}

int parse_statement(void) {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_IDENTIFIER) {
        current_token++;
        if (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR && 
            strcmp(tokens[current_token].value, "=") == 0) {
            current_token++;
            return parse_expression();
        }
    }
    
    return 0;
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize and parse: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("\nTokens found:\n");
    print_tokens();
    
    current_token = 0;
    printf("\nParsing result: ");
    if (parse_statement()) {
        printf("VALID expression\n");
    } else {
        printf("INVALID expression\n");
    }
    
    return 0;
}