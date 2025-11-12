//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    size_t count;
} TokenList;

TokenList* create_token_list(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    return list;
}

void destroy_token_list(TokenList* list) {
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
    size_t i = 0;
    size_t len = strlen(input);

    while (i < len) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        if (isdigit((unsigned char)input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit((unsigned char)input[i])) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            add_token(tokens, TOKEN_NUMBER, number);
            continue;
        }

        if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum((unsigned char)input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            add_token(tokens, TOKEN_IDENTIFIER, identifier);
            continue;
        }

        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            i++;
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
            }
            if (i < len && input[i] == '"') {
                i++;
            } else {
                fprintf(stderr, "Unterminated string\n");
                break;
            }
            string[j] = '\0';
            add_token(tokens, TOKEN_STRING, string);
            continue;
        }

        if (is_operator_char(input[i])) {
            char op[2] = {input[i], '\0'};
            add_token(tokens, TOKEN_OPERATOR, op);
            i++;
            continue;
        }

        fprintf(stderr, "Invalid character: %c\n", input[i]);
        i++;
    }

    add_token(tokens, TOKEN_EOF, "");
    return tokens;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(const TokenList* tokens) {
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        printf("Token %zu: %s", i + 1, token_type_to_string(token->type));
        if (token->value[0] != '\0') {
            printf(" ('%s')", token->value);
        }
        printf("\n");
    }
}

int main(void) {
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