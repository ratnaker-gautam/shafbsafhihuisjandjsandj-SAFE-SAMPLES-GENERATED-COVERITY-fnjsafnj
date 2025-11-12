//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"
#define IS_QUOTE(c) ((c) == '\"' || (c) == '\'')
#define IS_OPERATOR(c) ((c) == '+' || (c) == '-' || (c) == '*' || (c) == '/' || (c) == '=' || (c) == '<' || (c) == '>')

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
    if (list->count >= MAX_TOKENS || token == NULL) {
        return 0;
    }
    size_t len = strlen(token);
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(list->tokens[list->count], token, MAX_TOKEN_LEN - 1);
    list->tokens[list->count][MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

void print_tokens(const TokenList *list) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        printf("Token %d: '%s'\n", i + 1, list->tokens[i]);
    }
}

int is_delimiter(char c) {
    return strchr(DELIMITERS, c) != NULL;
}

int tokenize_string(TokenList *list, const char *input) {
    if (list == NULL || input == NULL) {
        return 0;
    }
    
    init_token_list(list);
    size_t len = strlen(input);
    if (len == 0) {
        return 1;
    }
    
    char buffer[MAX_TOKEN_LEN];
    int buf_index = 0;
    char in_quote = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        if (IS_QUOTE(c)) {
            if (in_quote == 0) {
                in_quote = c;
                if (buf_index > 0) {
                    buffer[buf_index] = '\0';
                    if (!add_token(list, buffer)) {
                        return 0;
                    }
                    buf_index = 0;
                }
                buffer[buf_index++] = c;
            } else if (in_quote == c) {
                buffer[buf_index++] = c;
                buffer[buf_index] = '\0';
                if (!add_token(list, buffer)) {
                    return 0;
                }
                buf_index = 0;
                in_quote = 0;
            } else {
                buffer[buf_index++] = c;
            }
        } else if (in_quote) {
            buffer[buf_index++] = c;
            if (buf_index >= MAX_TOKEN_LEN - 1) {
                return 0;
            }
        } else if (IS_OPERATOR(c)) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!add_token(list, buffer)) {
                    return 0;
                }
                buf_index = 0;
            }
            buffer[0] = c;
            buffer[1] = '\0';
            if (!add_token(list, buffer)) {
                return 0;
            }
        } else if (is_delimiter(c)) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!add_token(list, buffer)) {
                    return 0;
                }
                buf_index = 0;
            }
        } else {
            buffer[buf_index++] = c;
            if (buf_index >= MAX_TOKEN_LEN - 1) {
                return 0;
            }
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        if (!add_token(list, buffer)) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input[1024];
    TokenList tokens;
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!tokenize_string(&tokens, input)) {
        fprintf(stderr, "Error tokenizing input\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens(&tokens);
    printf("Total tokens: %d\n", tokens.count);
    
    return 0;
}