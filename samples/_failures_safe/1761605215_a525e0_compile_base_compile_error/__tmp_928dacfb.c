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

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0 || src == NULL) return;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    if (buf->size + len <= buf->capacity) {
        memcpy(buf->data + buf->size, src, len);
        buf->size += len;
    }
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    size_t output_size = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end && output_size + 2 <= output_capacity) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (output_size + 2 <= output_capacity) {
            *(output + output_size) = (uint8_t)count;
            *(output + output_size + 1) = value;
            output_size += 2;
        } else {
            break;
        }
        
        current = next;
    }
    
    return output_size;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    if (input_size % 2 != 0) {
        return 0;
    }
    
    size_t output_size = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t count = *current;
        uint8_t value = *(current + 1);
        
        if (output_size + count > output_capacity) {
            break;
        }
        
        uint8_t *dest = output + output_size;
        for (size_t i = 0; i < count; i++) {
            *(dest + i) = value;
        }
        
        output_size += count;
        current += 2;
    }
    
    return output_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    
    size_t compressed_size = compress_rle(input_data, input_size, compressed_data, sizeof(compressed_data));
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size > 0) {
        float compression_ratio = (1.0f - (float)compressed_size