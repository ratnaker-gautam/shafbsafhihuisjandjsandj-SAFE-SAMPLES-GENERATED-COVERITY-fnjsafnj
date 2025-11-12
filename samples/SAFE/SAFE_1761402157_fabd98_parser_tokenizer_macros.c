//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_PUNCTUATION,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    Token* token = &tokens[token_count++];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

int is_punctuation(char c) {
    return c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}';
}

void tokenize(const char* input) {
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;

    while (*p && token_count < MAX_TOKENS) {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        if (is_punctuation(*p)) {
            buffer[0] = *p;
            buffer[1] = '\0';
            add_token(TOKEN_PUNCTUATION, buffer);
            p++;
            continue;
        }

        if (isdigit((unsigned char)*p)) {
            buf_index = 0;
            while (*p && isdigit((unsigned char)*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
            continue;
        }

        if (isalpha((unsigned char)*p)) {
            buf_index = 0;
            while (*p && (isalnum((unsigned char)*p) || *p == '_') && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_WORD, buffer);
            continue;
        }

        p++;
    }
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_PUNCTUATION: printf("PUNCTUATION"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }

    tokenize(input);

    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
    }

    return 0;
}