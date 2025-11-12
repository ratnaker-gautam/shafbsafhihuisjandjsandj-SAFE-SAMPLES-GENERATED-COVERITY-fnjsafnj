//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return;
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        uint8_t run_length = (uint8_t)count;
        if (!buffer_append(output, &run_length, 1) || !buffer_append(output, &current, 1)) {
            return;
        }
        i += count;
    }
}

int read_input(uint8_t *buffer, size_t max_size, size_t *read_size) {
    if (buffer == NULL || read_size == NULL) return 0;
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    *read_size = total_read;
    return 1;
}

void write_output(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (!read_input(input_stack, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress(input_stack, input_size, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    write_output(&compressed);
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}