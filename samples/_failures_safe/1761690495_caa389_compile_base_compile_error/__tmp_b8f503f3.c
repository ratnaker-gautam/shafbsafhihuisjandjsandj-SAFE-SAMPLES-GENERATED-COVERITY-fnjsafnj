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
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenList;

TokenList* token_list_create(size_t initial_capacity) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

void token_list_destroy(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= list->capacity) return 0;
    
    if (list->count < MAX_TOKENS) {
        list->tokens[list->count] = token;
        list->count++;
        return 1;
    }
    return 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int parse_input(const char* input, TokenList* list) {
    if (!input || !list) return 0;
    
    size_t len = strlen(input);
    if (len == 0) return 1;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;
    
    for (size_t i = 0; i < len && list->count < MAX_TOKENS; i++) {
        if (isspace(input[i])) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                Token token;
                token.type = classify_token(buffer);
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_list_add(list, token)) return 0;
                buf_index = 0;
            }
            continue;
        }
        
        if (buf_index < MAX_TOKEN_LEN - 1) {
            buffer[buf_index++] = input[i];
        } else {
            return 0;
        }
        
        if (is_operator_char(input[i]) || input[i] == '"') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                Token token;
                token.type = classify_token(buffer);
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_list_add(list, token)) return 0;
                buf_index = 0;
            }
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        Token token;
        token.type = classify_token(buffer);
        strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
        token.value[MAX_TOKEN_LEN - 1] = '\0';
        if (!token_list_add(list, token)) return 0;
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.value, "EOF");
    return token_list_add(list, eof_token);
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const char* type_str;
        switch (list->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %zu: %s -> '%s'\n", i, type_str, list->tokens[i].value);
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr