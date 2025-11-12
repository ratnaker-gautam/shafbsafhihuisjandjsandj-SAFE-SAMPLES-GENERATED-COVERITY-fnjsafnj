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

int resize_buffer(Buffer *buf, size_t new_capacity) {
    if (new_capacity == 0 || new_capacity > MAX_INPUT_SIZE) {
        return 0;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    if (buf->size > new_capacity) {
        buf->size = new_capacity;
    }
    return 1;
}

void free_buffer(Buffer *buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer *buf) {
    size_t total_read = 0;
    uint8_t stack_buf[CHUNK_SIZE];
    
    while (1) {
        size_t bytes_read = fread(stack_buf, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            return 0;
        }
        
        if (total_read + bytes_read > MAX_INPUT_SIZE) {
            return 0;
        }
        
        if (buf->capacity < total_read + bytes_read) {
            size_t new_capacity = buf->capacity * 2;
            if (new_capacity < total_read + bytes_read) {
                new_capacity = total_read + bytes_read + CHUNK_SIZE;
            }
            if (new_capacity > MAX_INPUT_SIZE) {
                new_capacity = MAX_INPUT_SIZE;
            }
            if (!resize_buffer(buf, new_capacity)) {
                return 0;
            }
        }
        
        memcpy(buf->data + total_read, stack_buf, bytes_read);
        total_read += bytes_read;
    }
    
    buf->size = total_read;
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    if (input->size == 0) return;
    
    size_t max_output_size = input->size * 2;
    if (max_output_size > MAX_INPUT_SIZE) {
        max_output_size = MAX_INPUT_SIZE;
    }
    
    if (!resize_buffer(output, max_output_size)) {
        return;
    }
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input->size && out_pos < output->capacity - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current < 32 || current > 126) {
            output->data[out_pos++] = 0xFF;
            output->data[out_pos++] = count;
            output->data[out_pos++] = current;
        } else {
            for (size_t j = 0; j < count && out_pos < output->capacity; j++) {
                output->data[out_pos++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_pos;
}

int main(void) {
    Buffer input = create_buffer(CHUNK_SIZE);
    Buffer output = create_buffer(CHUNK_SIZE);
    
    if (!input.data || !output.data) {
        free_buffer(&input);
        free_buffer(&output);
        return 1;
    }
    
    if (!read_input(&input)) {
        free_buffer(&input);
        free_buffer(&output);
        return 1;
    }
    
    simple_compress(&input, &output);
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        free_buffer(&input);
        free_buffer(&output);
        return 1;
    }
    
    free_buffer(&input);
    free_buffer(&output);
    return 0;
}