//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
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
    if (buf != NULL) {
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
    if (buf->size > SIZE_MAX - data_size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) {
        return;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        uint8_t chunk[2] = { (uint8_t)count, current };
        if (!buffer_append(output, chunk, 2)) {
            return;
        }
        i += count;
    }
}

void simple_decompress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size % 2 != 0) {
        return;
    }
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) {
                return;
            }
        }
    }
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Buffer compressed, decompressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    if (!buffer_init(&decompressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    simple_compress((uint8_t*)input_str, input_len, &compressed);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    simple_decompress(compressed.data, compressed.size, &decompressed);
    if (decompressed.size == 0 || decompressed.size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    if (memcmp(input_str, decompressed.data, input_len) != 0) {
        fprintf(stderr, "Compression/decompression mismatch\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    double ratio = (1.0 - (double)compressed.size / (