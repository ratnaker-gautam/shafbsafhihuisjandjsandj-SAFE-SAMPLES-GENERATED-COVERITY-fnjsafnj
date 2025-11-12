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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf == NULL || data == NULL || len == 0) {
        return 0;
    }
    if (len > SIZE_MAX - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > SIZE_MAX / 2) {
            return 0;
        }
        if (buf->size + len > new_capacity) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return;
    }
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        uint8_t run_length = (uint8_t)count;
        if (!buffer_append(output, &run_length, 1) || !buffer_append(output, &current, 1)) {
            return;
        }
        i += count;
    }
}

void simple_decompress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL || input_len % 2 != 0) {
        return;
    }
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t run_length = input[i];
        uint8_t value = input[i + 1];
        for (uint8_t j = 0; j < run_length; j++) {
            if (!buffer_append(output, &value, 1)) {
                return;
            }
        }
    }
}

int main(void) {
    char stack_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(stack_input, sizeof(stack_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(stack_input);
    if (input_len > 0 && stack_input[input_len - 1] == '\n') {
        stack_input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    simple_compress((uint8_t*)stack_input, input_len, &compressed);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    Buffer decompressed;
    if (!buffer_init(&decompressed, input_len)) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    simple_decompress(compressed.data, compressed.size, &decompressed);
    if (decompressed.size == 0 || decompressed.size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    printf("Decompressed text: %s\n", decompressed.data);
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return 0;
}