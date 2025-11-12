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
    int line;
    int column;
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
    int capacity;
} TokenList;

TokenList* create_token_list() {
    TokenList* list = malloc(sizeof(TokenList));
    if (list == NULL) {
        return NULL;
    }
    list->count = 0;
    list->capacity = MAX_TOKENS;
    return list;
}

void free_token_list(TokenList* list) {
    free(list);
}

int add_token(TokenList* list, Token token) {
    if (list->count >= list->capacity) {
        return 0;
    }
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize_string(const char* input, TokenList* tokens) {
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);
    
    while (i < input_len && tokens->count < tokens->capacity) {
        char current = input[i];
        
        if (isspace(current)) {
            if (current == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }
        
        Token token;
        token.line = line;
        token.column = column;
        
        if (isdigit(current)) {
            token.type = TOKEN_NUMBER;
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        } else if (isalpha(current) || current == '_') {
            token.type = TOKEN_IDENTIFIER;
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        } else if (is_operator_char(current)) {
            token.type = TOKEN_OPERATOR;
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && is_operator_char(input[i])) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        } else if (current == '"') {
            token.type = TOKEN_STRING;
            int j = 0;
            i++;
            column++;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
        } else {
            i++;
            column++;
            continue;
        }
        
        if (!add_token(tokens, token)) {
            break;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    eof_token.line = line;
    eof_token.column = column;
    add_token(tokens, eof_token);
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

void print_tokens(TokenList* tokens) {
    printf("Tokens found:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < tokens->count; i++) {
        Token token = tokens->tokens[i];
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_to_string(token.type),
               token.value,
               token.line,
               token.column);
    }
}

int main() {
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\