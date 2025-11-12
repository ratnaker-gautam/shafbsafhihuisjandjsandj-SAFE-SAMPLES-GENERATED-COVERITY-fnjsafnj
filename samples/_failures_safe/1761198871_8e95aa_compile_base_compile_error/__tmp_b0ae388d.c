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

TokenType classify_token(const char* value) {
    if (value[0] == '\0') return TOKEN_EOF;
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    size_t len = strlen(input);
    size_t pos = 0;
    char buffer[MAX_TOKEN_LEN];
    
    while (pos < len && tokens->count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        size_t buf_pos = 0;
        char current = input[pos];
        
        if (is_operator_char(current)) {
            buffer[buf_pos++] = current;
            pos++;
        } else if (isdigit(current)) {
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                if (buf_pos >= MAX_TOKEN_LEN - 1) break;
                buffer[buf_pos++] = input[pos++];
            }
        } else if (isalpha(current) || current == '_') {
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (buf_pos >= MAX_TOKEN_LEN - 1) break;
                buffer[buf_pos++] = input[pos++];
            }
        } else if (current == '"') {
            buffer[buf_pos++] = current;
            pos++;
            while (pos < len && input[pos] != '"') {
                if (buf_pos >= MAX_TOKEN_LEN - 1) break;
                buffer[buf_pos++] = input[pos++];
            }
            if (pos < len && input[pos] == '"') {
                buffer[buf_pos++] = input[pos++];
            }
        } else {
            pos++;
            continue;
        }
        
        buffer[buf_pos] = '\0';
        Token token;
        token.type = classify_token(buffer);
        strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
        token.value[MAX_TOKEN_LEN - 1] = '\0';
        
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
    if (len > 0 && input[len - 1