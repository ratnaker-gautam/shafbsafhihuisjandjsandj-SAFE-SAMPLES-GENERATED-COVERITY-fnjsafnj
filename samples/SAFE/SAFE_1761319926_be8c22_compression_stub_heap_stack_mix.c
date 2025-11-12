//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.capacity = (initial_capacity > 0) ? initial_capacity : 1;
    buf.data = malloc(buf.capacity);
    if (buf.data == NULL) {
        buf.size = 0;
        buf.capacity = 0;
        return buf;
    }
    buf.size = 0;
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) {
        return 0;
    }
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity *= 2;
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count; i++) {
                output[out_idx++] = current;
            }
        }
        
        in_idx += count;
        
        if (out_idx >= CHUNK_SIZE - 3) {
            break;
        }
    }
    
    *output_size = out_idx;
}

int main(void) {
    uint8_t input_stack[CHUNK_SIZE];
    Buffer compressed = create_buffer(CHUNK_SIZE * 2);
    
    if (compressed.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t total_read = 0;
    
    while (1) {
        if (total_read >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            destroy_buffer(&compressed);
            return 1;
        }
        
        size_t bytes_read = fread(input_stack, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (ferror(stdin)) {
                fprintf(stderr, "Read error\n");
                destroy_buffer(&compressed);
                return 1;
            }
            break;
        }
        
        total_read += bytes_read;
        
        uint8_t output_stack[CHUNK_SIZE * 2];
        size_t compressed_size;
        
        compress_chunk(input_stack, bytes_read, output_stack, &compressed_size);
        
        if (!buffer_append(&compressed, output_stack, compressed_size)) {
            fprintf(stderr, "Compression buffer full\n");
            destroy_buffer(&compressed);
            return 1;
        }
    }
    
    if (fwrite(compressed.data, 1, compressed.size, stdout) != compressed.size) {
        fprintf(stderr, "Write error\n");
        destroy_buffer(&compressed);
        return 1;
    }
    
    destroy_buffer(&compressed);
    return 0;
}