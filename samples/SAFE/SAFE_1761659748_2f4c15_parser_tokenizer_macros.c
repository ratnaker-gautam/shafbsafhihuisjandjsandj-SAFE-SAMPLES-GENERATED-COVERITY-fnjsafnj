//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenList;

void init_token_list(TokenList *list) {
    list->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        list->tokens[i][0] = '\0';
    }
}

int add_token(TokenList *list, const char *token) {
    if (list->count >= MAX_TOKENS) return 0;
    if (strlen(token) >= MAX_TOKEN_LEN) return 0;
    
    strncpy(list->tokens[list->count], token, MAX_TOKEN_LEN - 1);
    list->tokens[list->count][MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

void tokenize_expression(TokenList *list, const char *input) {
    if (!list || !input) return;
    
    char buffer[MAX_TOKEN_LEN];
    int buf_pos = 0;
    int input_len = strlen(input);
    
    for (int i = 0; i < input_len && list->count < MAX_TOKENS; i++) {
        if (isspace((unsigned char)input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (!add_token(list, buffer)) break;
                buf_pos = 0;
            }
            continue;
        }
        
        if (IS_OPERATOR(input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (!add_token(list, buffer)) break;
                buf_pos = 0;
            }
            char op[2] = {input[i], '\0'};
            if (!add_token(list, op)) break;
            continue;
        }
        
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = input[i];
        }
    }
    
    if (buf_pos > 0 && list->count < MAX_TOKENS) {
        buffer[buf_pos] = '\0';
        add_token(list, buffer);
    }
}

void print_tokens(const TokenList *list) {
    if (!list) return;
    
    printf("Tokens (%d):\n", list->count);
    for (int i = 0; i < list->count; i++) {
        printf("[%d] '%s'\n", i, list->tokens[i]);
    }
}

int main() {
    char input[1024];
    TokenList tokens;
    
    init_token_list(&tokens);
    
    printf("Enter mathematical expression: ");
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
    
    tokenize_expression(&tokens, input);
    print_tokens(&tokens);
    
    return 0;
}