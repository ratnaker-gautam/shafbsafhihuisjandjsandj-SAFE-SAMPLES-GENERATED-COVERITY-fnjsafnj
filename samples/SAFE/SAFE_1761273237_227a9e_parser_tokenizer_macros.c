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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

TokenType classify_token(const char* str) {
    if (strlen(str) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (is_operator(str[0]) && strlen(str) == 1) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
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
        if (strlen(token_str) < MAX_TOKEN_LEN) {
            strcpy(tokens[token_count].value, token_str);
            tokens[token_count].type = classify_token(token_str);
            token_count++;
        }
        token_str = strtok(NULL, DELIMITERS);
    }
    
    free(copy);
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
    return 1;
}

Token get_next_token(void) {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

void reset_parser(void) {
    current_token = 0;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (!tokenize(input)) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens:\n");
    Token token;
    reset_parser();
    
    while ((token = get_next_token()).type != TOKEN_EOF) {
        printf("Type: %-12s Value: %s\n", 
               token_type_to_string(token.type), token.value);
    }
    
    return 0;
}