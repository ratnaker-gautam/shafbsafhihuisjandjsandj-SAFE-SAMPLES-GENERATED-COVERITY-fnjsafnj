//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
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
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len > MAX_INPUT_SIZE - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
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

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (count >= 3) {
            uint8_t run[2] = {current, (uint8_t)count};
            if (!buffer_append(output, run, 2)) {
                return;
            }
            i += count;
        } else {
            if (!buffer_append(output, &current, 1)) {
                return;
            }
            i++;
        }
    }
}

int read_input(Buffer *input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    if (!buffer_init(input, CHUNK_SIZE)) {
        return 0;
    }
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            buffer_free(input);
            return 0;
        }
    }
    
    if (ferror(stdin)) {
        buffer_free(input);
        return 0;
    }
    
    return 1;
}

int main(void) {
    Buffer input_data;
    Buffer compressed_data;
    
    if (!read_input(&input_data)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_data.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_data);
        return 1;
    }
    
    if (!buffer_init(&compressed_data, input_data.size)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_data);
        return 1;
    }
    
    simple_compress(input_data.data, input_data.size, &compressed_data);
    
    if (fwrite(compressed_data.data, 1, compressed_data.size, stdout) != compressed_data.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    buffer_free(&input_data);
    buffer_free(&compressed_data);
    
    return 0;
}