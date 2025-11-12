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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        while (strchr(DELIMITERS, *ptr)) ptr++;
        if (*ptr == '\0') break;
        
        size_t len = 0;
        TokenType type = classify_token(ptr);
        
        if (type == TOKEN_STRING) {
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            type = TOKEN_STRING;
        } else if (type == TOKEN_OPERATOR) {
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
        } else if (type == TOKEN_NUMBER) {
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
        } else if (type == TOKEN_IDENTIFIER) {
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
        } else {
            buffer[len++] = *ptr++;
        }
        
        if (len > 0) {
            buffer[len] = '\0';
            tokens[token_count].type = type;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
    }
    
    return 1;
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
    
    Token token = tokens[current_token];
    if (token.type == TOKEN_IDENTIFIER) {
        current_token++;
        if (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR) {
            current_token++;
            return parse_expression();
        }
    }
    return 0;
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s (%s)\n", i, tokens[i].value, 
               token_type_name(tokens[i].type));
    }
    
    current_token = 0;
    if (parse_statement()) {
        printf("Parse successful: Valid statement\n");
    } else {
        printf("Parse failed: Invalid statement\n");
    }
    
    return 0;
}