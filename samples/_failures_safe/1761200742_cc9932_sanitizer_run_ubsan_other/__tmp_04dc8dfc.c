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

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer *input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    if (!buffer_append(&chunk, (uint8_t*)"", 0)) {
        buffer_free(&chunk);
        return 0;
    }
    
    size_t total_read = 0;
    uint8_t read_buf[CHUNK_SIZE];
    
    while (1) {
        size_t bytes_read = fread(read_buf, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
        
        if (!buffer_append(&chunk, read_buf, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        
        if (bytes_read < CHUNK_SIZE) break;
    }
    
    if (ferror(stdin)) {
        buffer_free(&chunk);
        return 0;
    }
    
    *input = chunk;
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    buffer_init(output);
    
    if (input->size == 0) return;
    
    if (!buffer_append(output, (uint8_t*)"CMPR", 4)) return;
    
    uint32_t size = (uint32_t)input->size;
    if (size != input->size) return;
    
    uint8_t size_bytes[4];
    size_bytes[0] = (size >> 24) & 0xFF;
    size_bytes[1] = (size >> 16) & 0xFF;
    size_bytes[2] = (size >> 8) & 0xFF;
    size_bytes[3] = size & 0xFF;
    
    if (!buffer_append(output, size_bytes, 4)) return;
    
    for (size_t i = 0; i < input->size; i++) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + 1 < input->size && input->data[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            buffer_free(output);
            return;
        }
    }
}

int main(void) {
    Buffer input, output;
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    simple_compress(&input, &output);
    
    if (output.data && output.size > 0) {
        fwrite(output.data, 1, output.size, stdout);
    }
    
    buffer_free(&input);
    buffer_free(&output);
    
    return 0;
}