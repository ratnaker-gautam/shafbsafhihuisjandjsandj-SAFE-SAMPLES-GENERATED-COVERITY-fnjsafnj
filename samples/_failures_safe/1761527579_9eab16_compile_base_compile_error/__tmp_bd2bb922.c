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
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (src == NULL || len == 0 || len > MAX_OUTPUT_SIZE) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
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
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || output == NULL) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        uint8_t run[2] = {value, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        current = next;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || input_size > MAX_OUTPUT_SIZE || output == NULL) {
        return 0;
    }
    if (input_size % 2 != 0) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    while (current < end) {
        uint8_t value = *current;
        current++;
        uint8_t count = *current;
        current++;
        if (count == 0) {
            return 0;
        }
        for (uint8_t i = 0; i < count; i++) {
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t input_size = sizeof(input_data) / sizeof(input_data[0]);
    
    Buffer compressed;
    if (!buffer_init(&compressed, 32)) {
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
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    
    Buffer decompressed;
    if (!buffer_init(&decompressed, 64)) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_size) {
        fprintf(stderr, "Decomp