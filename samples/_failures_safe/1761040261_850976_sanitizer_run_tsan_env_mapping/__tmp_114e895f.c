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
    return isspace(c) || c == ',' || c == ';' || c == '.' || c == '!' || c == '?';
}

int tokenize_string(const char *input, TokenArray *token_arr) {
    if (input == NULL || token_arr == NULL) {
        return -1;
    }
    
    token_arr->count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && token_arr->count < MAX_TOKENS) {
        while (*ptr != '\0' && is_delimiter(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        const char *start = ptr;
        while (*ptr != '\0' && !is_delimiter(*ptr)) {
            ptr++;
        }
        
        size_t token_len = ptr - start;
        if (token_len >= MAX_TOKEN_LEN) {
            token_len = MAX_TOKEN_LEN - 1;
        }
        
        strncpy(token_arr->tokens[token_arr->count], start, token_len);
        token_arr->tokens[token_arr->count][token_len] = '\0';
        token_arr->count++;
        
        if (token_arr->count >= MAX_TOKENS) {
            break;
        }
    }
    
    return token_arr->count;
}

void print_tokens(const TokenArray *token_arr) {
    if (token_arr == NULL) {
        return;
    }
    
    printf("Found %d tokens:\n", token_arr->count);
    for (int i = 0; i < token_arr->count; i++) {
        printf("%d: '%s'\n", i + 1, token_arr->tokens[i]);
    }
}

int main(void) {
    DynamicBuffer input_buf;
    TokenArray tokens;
    char line[256];
    
    init_buffer(&input_buf, 64);
    
    printf("Enter text to tokenize (empty line to finish):\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        
        append_to_buffer(&input_buf, line);
        
        if (strlen(input_buf.data) > 1024) {
            printf("Input too long, processing...\n");
            break;
        }
    }
    
    if (input_buf.length == 0) {
        printf("No input provided.\n");
        free_buffer(&input_buf);
        return EXIT_FAILURE;
    }
    
    int token_count = tokenize_string(input_buf.data, &tokens);
    
    if (token_count < 0) {
        printf("Tokenization failed.\n");
        free_buffer(&input_buf);
        return EXIT_FAILURE;
    }
    
    print_tokens(&tokens);
    
    free_buffer(&input_buf);
    return EXIT_SUCCESS;
}