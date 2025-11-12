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
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    size_t remaining = data_size;
    
    while (remaining > 0) {
        *dest = *src;
        dest++;
        src++;
        remaining--;
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
        size_t run_length = 1;
        
        current++;
        while (current < end && *current == value && run_length < 255) {
            run_length++;
            current++;
        }
        
        uint8_t compressed[2] = { (uint8_t)run_length, value };
        if (!buffer_append(output, compressed, 2)) return 0;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t count = *current;
        current++;
        uint8_t value = *current;
        current++;
        
        if (output->size + count > output->capacity) {
            size_t new_capacity = output->capacity * 2;
            if (new_capacity < output->size + count) {
                new_capacity = output->size + count;
            }
            if (new_capacity > MAX_OUTPUT_SIZE) return 0;
            
            uint8_t *new_data = realloc(output->data, new_capacity);
            if (new_data == NULL) return 0;
            
            output->data = new_data;
            output->capacity = new_capacity;
        }
        
        uint8_t *dest = output->data + output->size;
        for (size_t i = 0; i < count; i++) {
            *dest = value;
            dest++;
        }
        output->size += count;
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size] = (uint8_t)c;
        input_size++;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, 64)) {
        printf("Failed to initialize compression buffer.\n");
        return 1;
    }
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio