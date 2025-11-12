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
    if (buf == NULL || data == NULL || len == 0 || len > SIZE_MAX - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return 0;
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

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_len = 0;
    for (size_t i = 0; i < input_len && output_len < output_capacity - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (output_len + 2 > output_capacity) {
            break;
        }
        output[output_len++] = (uint8_t)count;
        output[output_len++] = current;
        i += count;
    }
    return output_len;
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_stack, sizeof(input_stack), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    uint8_t *chunk_output = malloc(input_len * 2);
    if (chunk_output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&compressed);
        return 1;
    }
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        size_t compressed_size = simple_compress((uint8_t*)input_stack + i, chunk_size, chunk_output, input_len * 2);
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed\n");
            free(chunk_output);
            buffer_free(&compressed);
            return 1;
        }
        if (!buffer_append(&compressed, chunk_output, compressed_size)) {
            fprintf(stderr, "Buffer append failed\n");
            free(chunk_output);
            buffer_free(&compressed);
            return 1;
        }
    }
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) {
        printf("...");
    }
    printf("\n");
    free(chunk_output);
    buffer_free(&compressed);
    return 0;
}