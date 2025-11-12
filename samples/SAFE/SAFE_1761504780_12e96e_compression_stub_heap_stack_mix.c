//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.capacity = (initial_capacity > 0) ? initial_capacity : 1;
    buf.data = malloc(buf.capacity);
    if (!buf.data) {
        buf.size = 0;
        buf.capacity = 0;
        return buf;
    }
    buf.size = 0;
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + len) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void free_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (!input || !output || !out_len || len == 0 || len > CHUNK_SIZE) return 0;
    size_t j = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        if (j + 2 > CHUNK_SIZE) return 0;
        output[j++] = current;
        output[j++] = (uint8_t)count;
        i += count;
    }
    *out_len = j;
    return 1;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer heap_output = create_buffer(CHUNK_SIZE);
    if (!heap_output.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);

    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        size_t to_read = CHUNK_SIZE;
        if (total_read + to_read > MAX_INPUT_SIZE) {
            to_read = MAX_INPUT_SIZE - total_read;
        }
        size_t bytes_read = fread(stack_input, 1, to_read, stdin);
        if (bytes_read == 0) break;

        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_len = 0;
        if (!compress_chunk(stack_input, bytes_read, compressed, &compressed_len)) {
            fprintf(stderr, "Compression failed\n");
            free_buffer(&heap_output);
            return 1;
        }

        if (!buffer_append(&heap_output, compressed, compressed_len)) {
            fprintf(stderr, "Output buffer full\n");
            free_buffer(&heap_output);
            return 1;
        }

        total_read += bytes_read;
        if (bytes_read < to_read) break;
    }

    if (total_read == 0) {
        printf("No input data\n");
        free_buffer(&heap_output);
        return 1;
    }

    printf("Compressed data (%zu bytes):\n", heap_output.size);
    for (size_t i = 0; i < heap_output.size; i++) {
        printf("%02X", heap_output.data[i]);
        if ((i + 1) % 32 == 0) printf("\n");
    }
    printf("\n");

    double ratio = (total_read > 0) ? (double)heap_output.size / total_read : 0.0;
    printf("Compression ratio: %.2f\n", ratio);

    free_buffer(&heap_output);
    return 0;
}