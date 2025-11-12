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
        size_t new_cap = buf->capacity ? buf->capacity * 2 : CHUNK_SIZE;
        if (new_cap < new_size) new_cap = new_size;
        if (new_cap > MAX_INPUT_SIZE) return 0;
        uint8_t *new_data = realloc(buf->data, new_cap);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_cap;
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
        
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) return 0;
        
        i += count;
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
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (!simple_compress(input.data, input.size, &output)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    write_output(&output);
    
    buffer_free(&input);
    buffer_free(&output);
    
    return 0;
}