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
    if (buf == NULL || initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
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
    if (input == NULL || input_size == 0 || output == NULL) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE) {
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
        current += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) {
        return 0;
    }
    if (input_size % 2 != 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        if (count == 0) {
            return 0;
        }
        size_t i = 0;
        while (i < count) {
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
            i++;
        }
        current += 2;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[] = {1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 5, 5};
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
        fprintf(st