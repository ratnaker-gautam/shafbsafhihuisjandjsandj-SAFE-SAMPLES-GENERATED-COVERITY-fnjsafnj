//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation(char c) {
    return c == '.' || c == ',' || c == ';' || c == ':' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        for (int i = 0; str[i]; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (is_operator(str[0])) {
        if (str[1] == '\0') return TOKEN_OPERATOR;
        return TOKEN_UNKNOWN;
    }
    
    if (is_punctuation(str[0])) {
        if (str[1] == '\0') return TOKEN_PUNCTUATION;
        return TOKEN_UNKNOWN;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (int i = 0; str[i]; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_WORD;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    char buffer[MAX_TOKEN_LEN];
    char* copy = strdup(input);
    if (!copy) return -1;
    
    int token_count = 0;
    char* token = strtok(copy, DELIMITERS);
    
    while (token && token_count < max_tokens) {
        if (strlen(token) >= MAX_TOKEN_LEN) {
            free(copy);
            return -1;
        }
        
        strncpy(buffer, token, MAX_TOKEN_LEN - 1);
        buffer[MAX_TOKEN_LEN - 1] = '\0';
        
        tokens[token_count].type = classify_token(buffer);
        strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        
        token_count++;
        token = strtok(NULL, DELIMITERS);
    }
    
    free(copy);
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_PUNCTUATION: printf("PUNCTUATION"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main() {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Found %d tokens:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: '%-15s' Type: ", i + 1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}