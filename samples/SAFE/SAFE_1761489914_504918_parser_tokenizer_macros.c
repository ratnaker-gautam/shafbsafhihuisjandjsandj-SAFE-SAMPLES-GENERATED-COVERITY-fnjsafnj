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
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        return TOKEN_NUMBER;
    } else if (is_operator(str[0]) && str[1] == '\0') {
        return TOKEN_OPERATOR;
    } else if (isalpha(str[0])) {
        return TOKEN_WORD;
    }
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) {
        return -1;
    }

    char buffer[MAX_TOKEN_LEN * MAX_TOKENS];
    if (strlen(input) >= sizeof(buffer)) {
        return -1;
    }
    strcpy(buffer, input);

    int token_count = 0;
    char* token_str = strtok(buffer, DELIMITERS);

    while (token_str != NULL && token_count < max_tokens) {
        if (strlen(token_str) >= MAX_TOKEN_LEN) {
            return -1;
        }

        tokens[token_count].type = classify_token(token_str);
        strcpy(tokens[token_count].value, token_str);
        token_count++;
        token_str = strtok(NULL, DELIMITERS);
    }

    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    if (!tokens || count <= 0) {
        return;
    }

    for (int i = 0; i < count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_WORD: type_str = "WORD"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s (%s)\n", i + 1, tokens[i].value, type_str);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);

    if (token_count < 0) {
        fprintf(stderr, "Error during tokenization\n");
        return 1;
    }

    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }

    printf("Found %d tokens:\n", token_count);
    print_tokens(tokens, token_count);

    return 0;
}