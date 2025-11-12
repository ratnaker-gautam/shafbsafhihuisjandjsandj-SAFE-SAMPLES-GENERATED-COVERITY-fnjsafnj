//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef struct {
    char *data;
    size_t capacity;
    size_t length;
} DynamicString;

typedef struct {
    char *tokens[MAX_TOKENS];
    int count;
} TokenArray;

int init_string(DynamicString *str, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    str->data = malloc(initial_capacity);
    if (!str->data) return 0;
    str->data[0] = '\0';
    str->capacity = initial_capacity;
    str->length = 0;
    return 1;
}

void free_string(DynamicString *str) {
    if (str->data) free(str->data);
    str->data = NULL;
    str->capacity = 0;
    str->length = 0;
}

int append_char(DynamicString *str, char c) {
    if (str->length + 2 > str->capacity) {
        size_t new_capacity = str->capacity * 2;
        if (new_capacity < 16) new_capacity = 16;
        char *new_data = realloc(str->data, new_capacity);
        if (!new_data) return 0;
        str->data = new_data;
        str->capacity = new_capacity;
    }
    str->data[str->length++] = c;
    str->data[str->length] = '\0';
    return 1;
}

void init_token_array(TokenArray *arr) {
    for (int i = 0; i < MAX_TOKENS; i++) {
        arr->tokens[i] = NULL;
    }
    arr->count = 0;
}

void free_token_array(TokenArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->tokens[i]) free(arr->tokens[i]);
    }
    arr->count = 0;
}

int add_token(TokenArray *arr, const char *token) {
    if (arr->count >= MAX_TOKENS) return 0;
    size_t len = strlen(token);
    if (len >= MAX_TOKEN_LEN) return 0;
    char *copy = malloc(len + 1);
    if (!copy) return 0;
    strcpy(copy, token);
    arr->tokens[arr->count++] = copy;
    return 1;
}

int is_delimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '.' || c == '!' || c == '?';
}

int tokenize_string(const char *input, TokenArray *tokens) {
    if (!input || !tokens) return 0;
    
    DynamicString current_token;
    if (!init_string(&current_token, 16)) return 0;
    
    int in_token = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (is_delimiter(input[i])) {
            if (in_token && current_token.length > 0) {
                if (!add_token(tokens, current_token.data)) {
                    free_string(&current_token);
                    return 0;
                }
                current_token.length = 0;
                current_token.data[0] = '\0';
            }
            in_token = 0;
        } else {
            if (!append_char(&current_token, input[i])) {
                free_string(&current_token);
                return 0;
            }
            in_token = 1;
        }
    }
    
    if (in_token && current_token.length > 0) {
        if (!add_token(tokens, current_token.data)) {
            free_string(&current_token);
            return 0;
        }
    }
    
    free_string(&current_token);
    return 1;
}

int main(void) {
    char input_buffer[1024];
    printf("Enter text to tokenize: ");
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    TokenArray tokens;
    init_token_array(&tokens);
    
    if (!tokenize_string(input_buffer, &tokens)) {
        printf("Error during tokenization\n");
        free_token_array(&tokens);
        return 1;
    }
    
    printf("Tokens found: %d\n", tokens.count);
    for (int i = 0; i < tokens.count; i++) {
        printf("Token %d: '%s'\n", i + 1, tokens.tokens[i]);
    }
    
    free_token_array(&tokens);
    return 0;
}