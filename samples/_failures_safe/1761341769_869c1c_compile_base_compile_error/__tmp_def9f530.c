//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
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
        if (new_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity * sizeof(uint8_t));
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    for (size_t i = 0; i < data_size; i++) {
        *dest++ = *src++;
    }
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        const uint8_t *run_start = current;
        uint8_t value = *current;
        
        while (current < end && *current == value && (current - run_start) < 255) {
            current++;
        }
        
        size_t run_length = current - run_start;
        
        if (run_length >= 3) {
            uint8_t encoded[2] = {value, (uint8_t)run_length};
            if (!buffer_append(output, encoded, 2)) return 0;
        } else {
            if (!buffer_append(output, run_start, run_length)) return 0;
        }
    }
    
    return 1;
}

int read_input(uint8_t *buffer, size_t max_size, size_t *actual_size) {
    if (buffer == NULL || actual_size == NULL) return 0;
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    
    *actual_size = total_read;
    return 1;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    const uint8_t *current = data;
    const uint8_t *end = data + size;
    
    while (current < end) {
        printf("%02X ", *current);
        current++;
    }
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    if (!read_input(input_buffer, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_size)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    print_buffer_hex(input_buffer, input_size);
    
    if (!compress_rle(input_buffer, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    double ratio = (double)compressed.size / input_size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    buffer_free(&