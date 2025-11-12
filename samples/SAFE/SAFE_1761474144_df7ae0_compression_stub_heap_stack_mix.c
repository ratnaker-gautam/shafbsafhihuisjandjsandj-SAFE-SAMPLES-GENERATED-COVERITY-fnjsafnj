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

void buffer_free(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len < output_capacity - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[output_len++] = current;
            output[output_len++] = current;
            output[output_len++] = (uint8_t)count;
            i += count;
        } else {
            output[output_len++] = current;
            i++;
        }
    }
    
    return output_len;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer input_buffer = buffer_create(CHUNK_SIZE);
    Buffer output_buffer = buffer_create(CHUNK_SIZE);
    
    if (!input_buffer.data || !output_buffer.data) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_buffer);
        buffer_free(&output_buffer);
        return EXIT_FAILURE;
    }
    
    size_t bytes_read;
    while ((bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input_buffer, stack_input, bytes_read)) {
            fprintf(stderr, "Input buffer overflow\n");
            buffer_free(&input_buffer);
            buffer_free(&output_buffer);
            return EXIT_FAILURE;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Input read error\n");
        buffer_free(&input_buffer);
        buffer_free(&output_buffer);
        return EXIT_FAILURE;
    }
    
    if (input_buffer.size > 0) {
        size_t max_output_size = input_buffer.size * 2;
        if (max_output_size > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large for processing\n");
            buffer_free(&input_buffer);
            buffer_free(&output_buffer);
            return EXIT_FAILURE;
        }
        
        if (!buffer_append(&output_buffer, NULL, max_output_size)) {
            fprintf(stderr, "Output buffer allocation failed\n");
            buffer_free(&input_buffer);
            buffer_free(&output_buffer);
            return EXIT_FAILURE;
        }
        
        output_buffer.size = 0;
        size_t compressed_size = simple_compress(input_buffer.data, input_buffer.size, 
                                               output_buffer.data, output_buffer.capacity);
        
        if (compressed_size > 0) {
            size_t written = fwrite(output_buffer.data, 1, compressed_size, stdout);
            if (written != compressed_size) {
                fprintf(stderr, "Output write error\n");
                buffer_free(&input_buffer);
                buffer_free(&output_buffer);
                return EXIT_FAILURE;
            }
        }
    }
    
    buffer_free(&input_buffer);
    buffer_free(&output_buffer);
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Output flush error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}