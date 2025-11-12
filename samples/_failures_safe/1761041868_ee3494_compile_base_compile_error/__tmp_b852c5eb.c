//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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
        return -1;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return -1;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
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
    if (buf == NULL || data == NULL || data_size == 0 || data_size > SIZE_MAX - buf->size) {
        return -1;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return -1;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return -1;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 0;
}

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (out_idx + 2 > CHUNK_SIZE) {
            break;
        }
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    *output_size = out_idx;
}

int main(void) {
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
    Buffer input_buf;
    if (buffer_init(&input_buf, input_len) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    if (buffer_append(&input_buf, (const uint8_t *)input_str, input_len) != 0) {
        fprintf(stderr, "Buffer append failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    Buffer output_buf;
    if (buffer_init(&output_buf, input_len) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_buf.size; i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= input_buf.size) ? CHUNK_SIZE : input_buf.size - i;
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size;
        compress_chunk(input_buf.data + i, chunk_size, compressed_chunk, &compressed_size);
        if (compressed_size > 0) {
            if (buffer_append(&output_buf, compressed_chunk, compressed_size) != 0) {
                fprintf(stderr, "Output buffer append failed\n");
                buffer_free(&input_buf);
                buffer_free(&output_buf);
                return 1;
            }
            total_compressed += compressed_size;
        }
    }
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", output_buf.size);
    if (input_buf.size > 0 && output_buf.size <= input_buf.size) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)output_buf.size / input