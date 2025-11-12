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
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (strlen(str) == 1 && is_operator(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0) return 0;
    
    char* copy = malloc(len + 1);
    if (copy == NULL) return 0;
    strcpy(copy, input);
    
    char* token_str = strtok(copy, DELIMITERS);
    
    while (token_str != NULL && token_count < MAX_TOKENS) {
        if (strlen(token_str) < MAX_TOKEN_LEN) {
            tokens[token_count].type = classify_token(token_str);
            strncpy(tokens[token_count].value, token_str, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        token_str = strtok(NULL, DELIMITERS);
    }
    
    free(copy);
    return 1;
}

Token get_next_token(void) {
    if (current_token >= token_count) {
        Token eof = {TOKEN_EOF, ""};
        return eof;
    }
    return tokens[current_token++];
}

void print_token(Token token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("%s: '%s'\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
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
    
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("%zu. ", i + 1);
        print_token(tokens[i]);
    }
    
    printf("\nParsing sequence:\n");
    current_token = 0;
    Token token;
    do {
        token = get_next_token();
        print_token(token);
    } while (token.type != TOKEN_EOF);
    
    return 0;
}