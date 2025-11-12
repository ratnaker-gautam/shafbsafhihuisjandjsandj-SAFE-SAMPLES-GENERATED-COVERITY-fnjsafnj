//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input_size == 0) return;
    
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
    if (input_size == 0 || input_size % 2 != 0) return;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return;
        }
    }
}

int read_input(Buffer *buf) {
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
        
        if (!buffer_append(buf, chunk, bytes_read)) return 0;
    }
    
    return 1;
}

void write_output(const Buffer *buf) {
    if (buf->size == 0) return;
    size_t written = fwrite(buf->data, 1, buf->size, stdout);
    if (written != buf->size) exit(EXIT_FAILURE);
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    if (!buffer_init(&input, CHUNK_SIZE)) goto error;
    if (!buffer_init(&compressed, CHUNK_SIZE)) goto error;
    if (!buffer_init(&decompressed, CHUNK_SIZE)) goto error;
    
    if (!read_input(&input)) goto error;
    
    simple_compress(input.data, input.size, &compressed);
    
    simple_decompress(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size != input.size) goto error;
    if (memcmp(decompressed.data, input.data, input.size) != 0) goto error;
    
    write_output(&compressed);
    
    buffer_free(&input);
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return EXIT_SUCCESS;

error:
    buffer_free(&input);
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return EXIT_FAILURE;
}