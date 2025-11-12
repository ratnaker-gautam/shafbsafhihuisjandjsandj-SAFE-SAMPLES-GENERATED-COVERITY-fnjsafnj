//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

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
    if (buf != NULL) {
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
    if (input == NULL || output == NULL || input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        uint8_t run_length = (count > UINT8_MAX) ? UINT8_MAX : count;
        uint8_t compressed_chunk[2] = {run_length, current};
        
        if (!buffer_append(output, compressed_chunk, 2)) {
            return;
        }
        
        i += count;
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? len : 0;
}

void print_compressed(const Buffer *compressed) {
    if (compressed == NULL || compressed->size == 0) return;
    
    printf("Compressed data (%zu bytes):\n", compressed->size);
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 < compressed->size) {
            printf("[%u,%c] ", compressed->data[i], compressed->data[i + 1]);
        }
    }
    printf("\n");
    
    double ratio = (compressed->size > 0) ? (double)compressed->size / (compressed->size / 2 * compressed->data[0]) : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = read_input(input_buffer, MAX_INPUT_SIZE);
    
    if (input_size == 0) {
        printf("No input provided or input too large.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        printf("Failed to initialize compression buffer.\n");
        return 1;
    }
    
    simple_compress(input_buffer, input_size, &compressed);
    
    if (compressed.size == 0) {
        printf("Compression failed or produced no output.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    print_compressed(&compressed);
    buffer_free(&compressed);
    
    return 0;
}