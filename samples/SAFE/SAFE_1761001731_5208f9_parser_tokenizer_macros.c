//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
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
    if (list->count >= MAX_TOKENS) {
        return 0;
    }
    if (strlen(token) >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(list->tokens[list->count], token, MAX_TOKEN_LEN - 1);
    list->tokens[list->count][MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

void tokenize_string(TokenList *list, const char *input) {
    if (input == NULL) {
        return;
    }
    
    char buffer[MAX_TOKEN_LEN];
    int buf_index = 0;
    int input_len = strlen(input);
    
    for (int i = 0; i < input_len; i++) {
        if (buf_index >= MAX_TOKEN_LEN - 1) {
            buffer[buf_index] = '\0';
            add_token(list, buffer);
            buf_index = 0;
        }
        
        if (IS_OPERATOR(input[i])) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(list, buffer);
                buf_index = 0;
            }
            buffer[0] = input[i];
            buffer[1] = '\0';
            add_token(list, buffer);
        } else if (strchr(DELIMITERS, input[i]) != NULL) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(list, buffer);
                buf_index = 0;
            }
        } else {
            buffer[buf_index++] = input[i];
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        add_token(list, buffer);
    }
}

void print_tokens(const TokenList *list) {
    if (list == NULL) {
        return;
    }
    printf("Tokens found: %d\n", list->count);
    for (int i = 0; i < list->count; i++) {
        printf("Token %d: '%s'\n", i + 1, list->tokens[i]);
    }
}

int main(void) {
    char input[1024];
    TokenList tokens;
    
    init_token_list(&tokens);
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    tokenize_string(&tokens, input);
    print_tokens(&tokens);
    
    return 0;
}