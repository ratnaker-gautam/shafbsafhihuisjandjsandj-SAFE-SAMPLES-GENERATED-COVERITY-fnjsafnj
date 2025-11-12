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

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len > SIZE_MAX - buf->size) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity ? buf->capacity * 2 : CHUNK_SIZE;
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

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        uint8_t run_length = (uint8_t)count;
        if (!buffer_append(output, &run_length, 1)) return 0;
        if (!buffer_append(output, &current, 1)) return 0;
        
        i += count;
    }
    
    return 1;
}

int simple_decompress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0 || input_len % 2 != 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t run_length = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < run_length; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
        
        i += 2;
    }
    
    return 1;
}

int read_input(Buffer *buf) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) return 0;
        if (buf->size >= MAX_INPUT_SIZE) break;
    }
    
    if (ferror(stdin)) return 0;
    return 1;
}

void write_output(const Buffer *buf) {
    if (buf->size > 0) {
        fwrite(buf->data, 1, buf->size, stdout);
    }
}

int main(void) {
    Buffer input, compressed, decompressed;
    buffer_init(&input);
    buffer_init(&compressed);
    buffer_init(&decompressed);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
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
    
    if (!simple_compress(input.data, input.size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (!simple_decompress(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (input.size != decompressed.size || 
        memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Verification failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    double ratio = (double)compressed.size / input.size * 100.0;
    fprintf(stderr, "Original: %zu bytes, Compressed: %zu bytes (%.1f%%)\n", 
            input.size, compressed.size, ratio);
    
    write_output(&compressed);
    
    buffer_free(&input);
    buffer_free(&compressed);
    buffer_free(&decompressed