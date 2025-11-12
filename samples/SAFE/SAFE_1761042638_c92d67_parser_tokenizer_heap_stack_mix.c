//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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
} DynamicBuffer;

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenArray;

void init_buffer(DynamicBuffer *buf, size_t initial_capacity) {
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    buf->capacity = initial_capacity;
    buf->length = 0;
    buf->data[0] = '\0';
}

void append_to_buffer(DynamicBuffer *buf, const char *str) {
    size_t str_len = strlen(str);
    if (buf->length + str_len + 1 > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->length + str_len + 1) {
            new_capacity = buf->length + str_len + 1;
        }
        char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    strcpy(buf->data + buf->length, str);
    buf->length += str_len;
}

void free_buffer(DynamicBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->capacity = 0;
    buf->length = 0;
}

int is_delimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '\0';
}

int tokenize_string(const char *input, TokenArray *token_arr) {
    if (input == NULL || token_arr == NULL) {
        return -1;
    }
    
    token_arr->count = 0;
    const char *p = input;
    
    while (*p != '\0' && token_arr->count < MAX_TOKENS) {
        while (is_delimiter(*p)) {
            p++;
        }
        
        if (*p == '\0') {
            break;
        }
        
        const char *start = p;
        while (*p != '\0' && !is_delimiter(*p)) {
            p++;
        }
        
        size_t token_len = p - start;
        if (token_len >= MAX_TOKEN_LEN) {
            token_len = MAX_TOKEN_LEN - 1;
        }
        
        if (token_len > 0) {
            strncpy(token_arr->tokens[token_arr->count], start, token_len);
            token_arr->tokens[token_arr->count][token_len] = '\0';
            token_arr->count++;
        }
    }
    
    return token_arr->count;
}

void print_tokens(const TokenArray *token_arr) {
    if (token_arr == NULL) {
        return;
    }
    
    printf("Tokens found: %d\n", token_arr->count);
    for (int i = 0; i < token_arr->count; i++) {
        printf("Token %d: '%s'\n", i + 1, token_arr->tokens[i]);
    }
}

int main(void) {
    char input_line[1024];
    DynamicBuffer input_buffer;
    TokenArray tokens;
    
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
    }
    
    if (strlen(input_line) == 0) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    init_buffer(&input_buffer, 64);
    append_to_buffer(&input_buffer, input_line);
    
    int token_count = tokenize_string(input_buffer.data, &tokens);
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        free_buffer(&input_buffer);
        return EXIT_FAILURE;
    }
    
    print_tokens(&tokens);
    
    free_buffer(&input_buffer);
    return EXIT_SUCCESS;
}