//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > MAX_OUTPUT_SIZE) return 0;
    
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
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) new_capacity = buf->size + data_size;
        if (new_capacity > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    size_t i = 0;
    while (i < data_size) {
        *(dest + i) = *(src + i);
        i++;
    }
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        const uint8_t *run_start = current;
        uint8_t value = *current;
        
        while (current < end && *current == value && (current - run_start) < 255) {
            current++;
        }
        
        size_t run_length = current - run_start;
        
        if (run_length > 1) {
            uint8_t encoded[2] = {value, (uint8_t)run_length};
            if (!buffer_append(output, encoded, 2)) return 0;
        } else {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        current++;
        
        if (current < end && *current > 1) {
            size_t count = *current;
            current++;
            
            if (output->size + count > output->capacity) {
                size_t new_capacity = output->capacity * 2;
                if (new_capacity < output->size + count) new_capacity = output->size + count;
                if (new_capacity > MAX_OUTPUT_SIZE) return 0;
                
                uint8_t *new_data = realloc(output->data, new_capacity);
                if (new_data == NULL) return 0;
                
                output->data = new_data;
                output->capacity = new_capacity;
            }
            
            uint8_t *dest = output->data + output->size;
            size_t i = 0;
            while (i < count) {
                *(dest + i) = value;
                i++;
            }
            output->size += count;
        } else {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[] = {1, 1, 1, 2, 3, 3, 4, 5, 5, 5, 5, 6, 7, 7, 8, 9, 9, 9};
    size_t input_size = sizeof(input_data);
    
    Buffer compressed;
    if (!buffer_init(&compressed, 64)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Comp