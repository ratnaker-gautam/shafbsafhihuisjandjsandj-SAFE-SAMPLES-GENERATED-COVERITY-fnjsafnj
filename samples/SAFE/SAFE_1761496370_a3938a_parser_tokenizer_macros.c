//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0) return 1;
    
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            size_t i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            size_t i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (*ptr == '"') {
            ptr++;
            size_t i = 0;
            while (*ptr && *ptr != '"' && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            size_t i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
    return 1;
}

Token get_next_token(void) {
    if (current_token >= token_count) {
        Token eof_token = {TOKEN_EOF, ""};
        return eof_token;
    }
    return tokens[current_token++];
}

void print_token(Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Type: %-12s Value: %s\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    current_token = 0;
    Token token;
    
    do {
        token = get_next_token();
        print_token(token);
    } while (token.type != TOKEN_EOF);
    
    return 0;
}