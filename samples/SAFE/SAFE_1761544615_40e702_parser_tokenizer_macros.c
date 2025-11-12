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
    
    if (isdigit(str[0])) {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len < 2 || str[len-1] != '"') return TOKEN_UNKNOWN;
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    char* copy = malloc(strlen(input) + 1);
    if (copy == NULL) return 0;
    strcpy(copy, input);
    
    char* token_str = strtok(copy, DELIMITERS);
    token_count = 0;
    
    while (token_str != NULL && token_count < MAX_TOKENS) {
        size_t len = strlen(token_str);
        if (len >= MAX_TOKEN_LEN) {
            free(copy);
            return 0;
        }
        
        tokens[token_count].type = classify_token(token_str);
        strncpy(tokens[token_count].value, token_str, MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        
        token_count++;
        token_str = strtok(NULL, DELIMITERS);
    }
    
    free(copy);
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

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int parse_expression(void) {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_NUMBER || 
        tokens[current_token].type == TOKEN_IDENTIFIER) {
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
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens();
    
    current_token = 0;
    if (parse_statement()) {
        printf("\nParse successful: Valid assignment statement\n");
    } else {
        printf("\nParse failed: Invalid syntax\n");
    }
    
    return 0;
}