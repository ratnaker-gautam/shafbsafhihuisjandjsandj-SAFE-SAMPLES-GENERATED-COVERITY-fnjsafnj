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
    for (size_t i = 0; i < data_size; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size > MAX_INPUT_SIZE) {
        return 0;
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
        if (run_length > 3 || value == 0xFF) {
            uint8_t encoded[2] = {0xFF, (uint8_t)run_length};
            if (!buffer_append(output, encoded, 2)) {
                return 0;
            }
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
        } else {
            if (!buffer_append(output, run_start, run_length)) {
                return 0;
            }
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    while (current < end) {
        if (*current == 0xFF && current + 2 < end) {
            uint8_t run_length = *(current + 1);
            uint8_t value = *(current + 2);
            if (run_length == 0) {
                return 0;
            }
            for (size_t i = 0; i < run_length; i++) {
                if (!buffer_append(output, &value, 1)) {
                    return 0;
                }
            }
            current += 3;
        } else {
            if (!buffer_append(output, current, 1)) {
                return 0;
            }
            current++;
        }
    }
    return 1;
}

int main() {
    uint8_t input_data[] = {0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04};
    size_t input_size = sizeof(input_data) / sizeof(input_data[0]);
    
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
    
    Buffer decompressed;
    if (!buffer_init(&decompressed, 64)) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {