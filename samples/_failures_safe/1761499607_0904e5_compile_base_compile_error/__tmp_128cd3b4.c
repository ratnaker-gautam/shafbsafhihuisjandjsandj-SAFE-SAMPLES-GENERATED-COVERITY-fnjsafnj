//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

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
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > MAX_INPUT_SIZE) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        uint8_t chunk[2] = { (uint8_t)count, current };
        if (!buffer_append(output, chunk, 2)) return 0;
        i += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    if (input_len % 2 != 0) return 0;
    
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        if (count == 0) return 0;
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    return 1;
}

int read_input(Buffer *buf) {
    if (!buf) return 0;
    if (!buffer_init(buf, CHUNK_SIZE)) return 0;
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) {
            buffer_free(buf);
            return 0;
        }
        if (buf->size >= MAX_INPUT_SIZE) break;
    }
    if (ferror(stdin)) {
        buffer_free(buf);
        return 0;
    }
    return 1;
}

int write_output(const Buffer *buf) {
    if (!buf || !buf->data) return 0;
    size_t written = fwrite(buf->data, 1, buf->size, stdout);
    if (written != buf->size) return 0;
    return 1;
}

int main(void) {
    Buffer input, compressed, decompressed;
    memset(&input, 0, sizeof(Buffer));
    memset(&compressed, 0, sizeof(Buffer));
    memset(&decompressed, 0, sizeof(Buffer));
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!buffer_init(&compressed, input.size * 2)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    if (!compress_rle(input.data, input.size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return 1;
    }
    
    if (!buffer_init(&decompressed, input.size)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return 1;
    }
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decomp