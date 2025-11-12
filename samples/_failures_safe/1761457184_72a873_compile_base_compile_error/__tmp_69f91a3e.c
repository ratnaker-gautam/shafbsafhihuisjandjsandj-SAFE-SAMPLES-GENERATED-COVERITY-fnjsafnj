//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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
    int line;
    int column;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenList;

void token_list_init(TokenList* list) {
    list->capacity = 16;
    list->count = 0;
    list->tokens = malloc(list->capacity * sizeof(Token));
    if (!list->tokens) exit(EXIT_FAILURE);
}

void token_list_add(TokenList* list, Token token) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap > MAX_TOKENS) new_cap = MAX_TOKENS;
        Token* new_tokens = realloc(list->tokens, new_cap * sizeof(Token));
        if (!new_tokens) exit(EXIT_FAILURE);
        list->tokens = new_tokens;
        list->capacity = new_cap;
    }
    if (list->count < MAX_TOKENS) {
        list->tokens[list->count++] = token;
    }
}

void token_list_free(TokenList* list) {
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

int tokenize(const char* input, TokenList* tokens) {
    int line = 1;
    int column = 1;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        if (i >= len) break;
        
        Token token = {0};
        token.line = line;
        token.column = column;
        size_t token_len = 0;
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            token.value[token_len++] = input[i++];
            column++;
            while (i < len && input[i] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
                column++;
            }
            if (i < len && input[i] == '"') {
                token.value[token_len++] = input[i++];
                column++;
            }
        } else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            while (i < len && is_operator_char(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
                column++;
            }
        } else if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (i < len && (isdigit(input[i]) || input[i] == '.') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
                column++;
            }
        } else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            while (i < len && (isalnum(input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
                column++;
            }
        } else {
            i++;
            column++;
            continue;
        }
        
        token.value[token_len] = '\0';
        if (token_len > 0) {
            token_list_add(tokens, token);
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line, column};
    token_list_add(tokens, eof_token);
    
    return 1;
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
    
    if (!fgets(input, sizeof(input), stdin