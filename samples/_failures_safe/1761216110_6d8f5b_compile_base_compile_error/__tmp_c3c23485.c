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

int token_list_add(TokenList* list, TokenType type, const char* value) {
    if (!list || !value) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (list->count >= MAX_TOKENS) return 0;
    
    Token* token = &list->tokens[list->count];
    token->type = type;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            token_list_add(tokens, TOKEN_NUMBER, number);
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            token_list_add(tokens, TOKEN_IDENTIFIER, identifier);
        }
        else if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            i++;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
            }
            string[j] = '\0';
            if (i < len && input[i] == '"') i++;
            token_list_add(tokens, TOKEN_STRING, string);
        }
        else if (is_operator_char(input[i])) {
            char op[2] = {input[i++], '\0'};
            token_list_add(tokens, TOKEN_OPERATOR, op);
        }
        else {
            i++;
        }
    }
    
    token_list_add(tokens, TOKEN_EOF, "");
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    TokenList* tokens =