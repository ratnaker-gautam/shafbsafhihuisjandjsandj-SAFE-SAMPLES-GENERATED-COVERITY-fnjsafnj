//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) return 1;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        uint8_t run_length = (uint8_t)count;
        if (!buffer_append(output, &run_length, 1)) return;
        if (!buffer_append(output, &current, 1)) return;
        i += count;
    }
}

void simple_decompress(const uint8_t *input, size_t input_size, Buffer *output) {
    size_t i = 0;
    while (i + 1 < input_size) {
        uint8_t run_length = input[i];
        uint8_t value = input[i + 1];
        for (size_t j = 0; j < run_length; j++) {
            if (!buffer_append(output, &value, 1)) return;
        }
        i += 2;
    }
}

int read_input(Buffer *input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            return 0;
        }
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) return 0;
        if (!buffer_append(input, chunk, bytes_read)) return 0;
    }
    return 1;
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    if (!buffer_init(&input, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    if (!buffer_init(&decompressed, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return 1;
    }
    
    if (!read_input(&input)) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    simple_compress(input.data, input.size, &compressed);
    simple_decompress(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size != input.size) {
        fprintf(stderr, "Decompression size mismatch\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Decompression data mismatch\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    if (written != compressed.size) {
        fprintf(stderr, "Output write failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return 0;
}