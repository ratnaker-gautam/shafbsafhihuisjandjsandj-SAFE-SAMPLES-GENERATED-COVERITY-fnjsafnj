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

TokenList* create_token_list(size_t initial_capacity) {
    if (initial_capacity == 0) {
        return NULL;
    }
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) {
        return NULL;
    }
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->capacity = initial_capacity;
    list->count = 0;
    return list;
}

void destroy_token_list(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) {
        return 0;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) {
            new_capacity = MAX_TOKENS;
        }
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) {
            return 0;
        }
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') {
        return TOKEN_EOF;
    }
    
    if (isdigit(str[0])) {
        return TOKEN_NUMBER;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"') {
        return TOKEN_STRING;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_EOF;
}

TokenList* tokenize(const char* input) {
    if (!input) {
        return NULL;
    }
    
    TokenList* tokens = create_token_list(10);
    if (!tokens) {
        return NULL;
    }
    
    const char* pos = input;
    
    while (*pos && tokens->count < MAX_TOKENS) {
        while (isspace(*pos)) {
            pos++;
        }
        
        if (*pos == '\0') {
            break;
        }
        
        Token token;
        token.type = classify_token(pos);
        
        size_t len = 0;
        
        switch (token.type) {
            case TOKEN_NUMBER:
                while (isdigit(pos[len])) {
                    len++;
                }
                break;
                
            case TOKEN_IDENTIFIER:
                while (isalnum(pos[len]) || pos[len] == '_') {
                    len++;
                }
                break;
                
            case TOKEN_OPERATOR:
                len = 1;
                break;
                
            case TOKEN_STRING:
                len = 1;
                while (pos[len] && pos[len] != '"') {
                    len++;
                }
                if (pos[len] == '"') {
                    len++;
                }
                break;
                
            default:
                pos++;
                continue;
        }
        
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(token.value, pos, len);
            token.value[len] = '\0';
            
            if (token_list_add(tokens, token)) {
                pos += len;
            } else {
                break;
            }
        } else {
            pos++;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return tokens;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) {
        return;
    }
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING: %s\n", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: