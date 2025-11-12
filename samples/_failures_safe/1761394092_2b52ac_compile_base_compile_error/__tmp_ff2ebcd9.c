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

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t count) {
    if (count == 0 || src == NULL) return;
    
    if (buf->size > SIZE_MAX - count) {
        exit(EXIT_FAILURE);
    }
    
    size_t new_size = buf->size + count;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                exit(EXIT_FAILURE);
            }
            new_capacity *= 2;
            if (new_capacity > MAX_OUTPUT_SIZE) {
                exit(EXIT_FAILURE);
            }
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, src, count);
    buf->size = new_size;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        const uint8_t *run_start = current;
        uint8_t value = *current;
        
        while (current < end && *current == value && (current - run_start) < 255) {
            current++;
        }
        
        size_t run_length = current - run_start;
        if (run_length > 255) {
            run_length = 255;
            current = run_start + 255;
        }
        
        uint8_t encoded[2] = { (uint8_t)run_length, value };
        buffer_append(output, encoded, 2);
        
        if (output->size > MAX_OUTPUT_SIZE) {
            return -1;
        }
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    if (input_size % 2 != 0) {
        return -1;
    }
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t count = *current++;
        uint8_t value = *current++;
        
        if (count == 0) {
            return -1;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
            if (output->size > MAX_OUTPUT_SIZE) {
                return -1;
            }
        }
    }
    
    return 0;
}

int main(void) {
    uint8_t original_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 6};
    size_t original_size = sizeof(original_data);
    
    printf("Original data (%zu bytes): ", original_size);
    for (size_t i = 0; i < original_size; i++) {
        printf("%d ", original_data[i]);
    }
    printf("\n");
    
    Buffer compressed;
    buffer_init(&compressed, 32);
    
    if (compress_rle(original_data, original_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed;
    buffer_init(&decompressed, 64);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decomp