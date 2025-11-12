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
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenList;

TokenList* token_list_create(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    list->capacity = 16;
    list->count = 0;
    list->tokens = malloc(list->capacity * sizeof(Token));
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    return list;
}

void token_list_destroy(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    if (list->count >= MAX_TOKENS) return 0;
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, TokenList* tokens) {
    const char* p = input;
    while (*p && tokens->count < MAX_TOKENS) {
        while (isspace(*p)) p++;
        if (!*p) break;

        Token token = {0};
        if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        } else if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        } else if (*p == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            p++;
            while (*p != '"' && *p && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
            if (*p == '"') p++;
        } else if (is_operator_char(*p)) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        } else {
            p++;
            continue;
        }
        if (!token_list_add(tokens, token)) break;
    }
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';

    TokenList* tokens = token_list_create();
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    tokenize(input, tokens);

    printf("Tokens:\n");
    for (size_t i = 0; i < tokens->count; i++) {
        printf("  %s: '%s'\n", token_type_name(tokens->tokens[i].type), tokens->tokens[i].value);
    }

    token_list_destroy(tokens);
    return 0;
}