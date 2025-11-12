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

Buffer buffer_create(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= MAX_INPUT_SIZE) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_resize(Buffer *buf, size_t new_capacity) {
    if (new_capacity == 0 || new_capacity > MAX_INPUT_SIZE) {
        return 0;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    if (buf->size > buf->capacity) {
        buf->size = buf->capacity;
    }
    return 1;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        if (!buffer_resize(buf, new_capacity)) {
            return 0;
        }
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size && output_size < output_capacity - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        if (count >= 3) {
            if (output_size + 2 > output_capacity) {
                break;
            }
            output[output_size++] = current;
            output[output_size++] = (uint8_t)count;
            i += count;
        } else {
            if (output_size + 1 > output_capacity) {
                break;
            }
            output[output_size++] = current;
            i++;
        }
    }
    return output_size;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer input_buffer = buffer_create(CHUNK_SIZE);
    Buffer output_buffer = buffer_create(CHUNK_SIZE);
    
    if (!input_buffer.data || !output_buffer.data) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_buffer);
        buffer_free(&output_buffer);
        return 1;
    }
    
    size_t total_read = 0;
    while (1) {
        size_t bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (ferror(stdin)) {
                fprintf(stderr, "Read error\n");
                buffer_free(&input_buffer);
                buffer_free(&output_buffer);
                return 1;
            }
            break;
        }
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&input_buffer);
            buffer_free(&output_buffer);
            return 1;
        }
        if (!buffer_append(&input_buffer, stack_input, bytes_read)) {
            fprintf(stderr, "Buffer append failed\n");
            buffer_free(&input_buffer);
            buffer_free(&output_buffer);
            return 1;
        }
    }
    
    if (input_buffer.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_buffer);
        buffer_free(&output_buffer);
        return 1;
    }
    
    if (!buffer_resize(&output_buffer, input_buffer.size * 2)) {
        fprintf(stderr, "Output buffer resize failed\n");
        buffer_free(&input_buffer);
        buffer_free(&output_buffer);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_buffer.data, input_buffer.size, output_buffer.data, output_buffer.capacity);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buffer);
        buffer_free(&output_buffer);
        return 1;
    }
    
    size_t written = fwrite(output_buffer.data, 1, compressed_size,