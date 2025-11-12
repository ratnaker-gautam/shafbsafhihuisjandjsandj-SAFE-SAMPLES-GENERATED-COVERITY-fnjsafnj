//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenList;

TokenList* create_token_list() {
    TokenList* list = malloc(sizeof(TokenList));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    return list;
}

void free_token_list(TokenList* list) {
    free(list);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(TokenList* list, TokenType type, const char* value) {
    if (list->count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return;
    }
    if (strlen(value) >= MAX_TOKEN_LEN) {
        fprintf(stderr, "Token too long\n");
        return;
    }
    Token* token = &list->tokens[list->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
}

TokenList* tokenize(const char* input) {
    TokenList* tokens = create_token_list();
    int i = 0;
    int len = strlen(input);

    while (i < len) {
        if (isspace(input[i])) {
            i++;
            continue;
        }

        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            add_token(tokens, TOKEN_NUMBER, number);
            continue;
        }

        if (isalpha(input[i])) {
            char identifier[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && (isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            add_token(tokens, TOKEN_IDENTIFIER, identifier);
            continue;
        }

        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int j = 0;
            i++;
            while (i < len && input[i] != '"' && j < MAX_TOKEN_LEN - 1) {
                string[j++] = input[i++];
            }
            if (i < len && input[i] == '"') {
                i++;
            }
            string[j] = '\0';
            add_token(tokens, TOKEN_STRING, string);
            continue;
        }

        if (is_operator_char(input[i])) {
            char op[2] = {input[i++], '\0'};
            add_token(tokens, TOKEN_OPERATOR, op);
            continue;
        }

        i++;
    }

    add_token(tokens, TOKEN_EOF, "");
    return tokens;
}

void print_tokens(const TokenList* tokens) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };

    for (int i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        printf("Token %d: %s -> '%s'\n", i, type_names[token->type], token->value);
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return EXIT_FAILURE;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }

    TokenList* tokens = tokenize(input);
    print_tokens(tokens);
    free_token_list(tokens);

    return EXIT_SUCCESS;
}