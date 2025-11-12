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

void destroy_token_list(TokenList* list) {
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
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || !str[0]) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    
    return TOKEN_EOF;
}

int parse_token(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    
    size_t start = *pos;
    while (input[start] && isspace(input[start])) start++;
    
    if (!input[start]) {
        *pos = start;
        return 0;
    }
    
    token->type = classify_token(input + start);
    
    size_t len = 0;
    if (token->type == TOKEN_NUMBER) {
        while (input[start + len] && isdigit(input[start + len])) len++;
    } else if (token->type == TOKEN_IDENTIFIER) {
        while (input[start + len] && (isalnum(input[start + len]) || input[start + len] == '_')) len++;
    } else if (token->type == TOKEN_OPERATOR) {
        len = 1;
    } else if (token->type == TOKEN_STRING) {
        len = 1;
        while (input[start + len] && input[start + len] != '"') len++;
        if (input[start + len] == '"') len++;
    } else {
        return 0;
    }
    
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    *pos = start + len;
    return 1;
}

void print_token_type(TokenType type) {
    const char* names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"};
    if (type >= TOKEN_NUMBER && type <= TOKEN_EOF) {
        printf("%s", names[type]);
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
    
    TokenList* token_list = create_token_list(10);
    if (!token_list) {
        fprintf(stderr, "Failed to create token list\n");
        return 1;
    }
    
    size_t pos = 0;
    Token token;
    
    while (parse_token(input, &pos, &token)) {
        if (!token_list_add(token_list, token)) {
            fprintf(stderr, "Failed to add token to list\n");
            destroy_token_list(token_list);
            return 1;
        }
    }
    
    printf("Tokens found (%zu):\n", token_list->count);
    for (size_t i = 0; i < token_list->count; i++) {
        printf("Token %zu: Type: ", i);
        print_token_type(token_list->tokens[i].type);
        printf(", Value: \"%