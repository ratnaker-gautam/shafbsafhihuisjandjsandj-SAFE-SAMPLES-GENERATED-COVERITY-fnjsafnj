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
    if (initial_capacity == 0) return NULL;
    
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
    if (!list) return 0;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) return 0;
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
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
    if (!str || str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    
    if (str[0] == '"') return TOKEN_STRING;
    
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    const char* pos = input;
    
    while (*pos != '\0') {
        while (isspace((unsigned char)*pos)) pos++;
        
        if (*pos == '\0') break;
        
        Token token = {TOKEN_EOF, ""};
        size_t len = 0;
        
        token.type = classify_token(pos);
        
        if (token.type == TOKEN_NUMBER) {
            while (isdigit((unsigned char)*pos) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *pos++;
            }
        }
        else if (token.type == TOKEN_IDENTIFIER) {
            while ((isalnum((unsigned char)*pos) || *pos == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *pos++;
            }
        }
        else if (token.type == TOKEN_OPERATOR) {
            while (is_operator_char(*pos) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *pos++;
            }
        }
        else if (token.type == TOKEN_STRING) {
            token.value[len++] = *pos++;
            while (*pos != '"' && *pos != '\0' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *pos++;
            }
            if (*pos == '"') {
                token.value[len++] = *pos++;
            }
        }
        else {
            break;
        }
        
        token.value[len] = '\0';
        
        if (!token_list_add(tokens, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return 1;
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
    
    TokenList* tokens = token_list_create(10);
    if (!tokens