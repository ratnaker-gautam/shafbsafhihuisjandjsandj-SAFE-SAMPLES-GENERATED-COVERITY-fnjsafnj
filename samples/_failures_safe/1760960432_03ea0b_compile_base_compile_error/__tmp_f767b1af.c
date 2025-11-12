//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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
    if (src == NULL || len == 0) {
        return;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src_end = src + len;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return -1;
    }
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        uint8_t run[2] = {value, (uint8_t)count};
        buffer_append(output, run, 2);
        
        current = next;
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end - 1) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (count == 0) {
            return -1;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
        
        current += 2;
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_len) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1 || temp > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)temp;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    if (compress_rle(input_data, input_len, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", compressed.data[i]);
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
    
    printf("Decompressed data