//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= MAX_INPUT_SIZE) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0 || len > MAX_INPUT_SIZE) return 0;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > MAX_INPUT_SIZE) return 0;
        
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
    
    size_t pos = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (pos + 2 > CHUNK_SIZE) return 0;
        
        output[pos++] = count;
        output[pos++] = current;
        i += count;
    }
    
    *out_len = pos;
    return 1;
}

int main(void) {
    Buffer input_buf = create_buffer(CHUNK_SIZE);
    if (!input_buf.data) {
        fprintf(stderr, "Failed to allocate input buffer\n");
        return 1;
    }
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input_buf, chunk, bytes_read)) {
            fprintf(stderr, "Failed to append to input buffer\n");
            free_buffer(&input_buf);
            return 1;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    Buffer output_buf = create_buffer(input_buf.size);
    if (!output_buf.data) {
        fprintf(stderr, "Failed to allocate output buffer\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    for (size_t i = 0; i < input_buf.size; i += CHUNK_SIZE) {
        size_t chunk_len = (input_buf.size - i < CHUNK_SIZE) ? input_buf.size - i : CHUNK_SIZE;
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_len;
        
        if (compress_chunk(input_buf.data + i, chunk_len, compressed, &compressed_len)) {
            if (!buffer_append(&output_buf, compressed, compressed_len)) {
                fprintf(stderr, "Failed to append compressed data\n");
                free_buffer(&input_buf);
                free_buffer(&output_buf);
                return 1;
            }
        } else {
            fprintf(stderr, "Compression failed for chunk\n");
            free_buffer(&input_buf);
            free_buffer(&output_buf);
            return 1;
        }
    }
    
    if (fwrite(output_buf.data, 1, output_buf.size, stdout) != output_buf.size) {
        fprintf(stderr, "Error writing output\n");
        free_buffer(&input_buf);
        free_buffer(&output_buf);
        return 1;
    }
    
    free_buffer(&input_buf);
    free_buffer(&output_buf);
    return 0;
}