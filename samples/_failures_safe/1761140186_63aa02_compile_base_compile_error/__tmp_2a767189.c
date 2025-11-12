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
    size_t capacity;
    size_t count;
} TokenList;

TokenList* token_list_create(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->capacity = initial_capacity;
    list->count = 0;
    return list;
}

void token_list_destroy(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize_expression(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    size_t i = 0;
    char current_token[MAX_TOKEN_LEN] = {0};
    size_t token_pos = 0;
    
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }
        
        if (isdigit((unsigned char)input[i])) {
            token_pos = 0;
            while (isdigit((unsigned char)input[i]) && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            current_token[token_pos] = '\0';
            
            Token token = {TOKEN_NUMBER, ""};
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token_list_add(tokens, token);
        }
        else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            token_pos = 0;
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            current_token[token_pos] = '\0';
            
            Token token = {TOKEN_IDENTIFIER, ""};
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token_list_add(tokens, token);
        }
        else if (is_operator_char(input[i])) {
            token_pos = 0;
            while (is_operator_char(input[i]) && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            current_token[token_pos] = '\0';
            
            Token token = {TOKEN_OPERATOR, ""};
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token_list_add(tokens, token);
        }
        else if (input[i] == '"') {
            token_pos = 0;
            i++;
            while (input[i] != '"' && input[i] != '\0' && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            if (input[i] == '"') i++;
            current_token[token_pos] = '\0';
            
            Token token = {TOKEN_STRING, ""};
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token_list_add(tokens, token);
        }
        else {
            i++;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
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

int main(void) {
    char input[102