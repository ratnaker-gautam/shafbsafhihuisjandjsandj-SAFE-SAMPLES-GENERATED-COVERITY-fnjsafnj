//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.capacity = (initial_capacity > 0) ? initial_capacity : 1;
    buf.data = malloc(buf.capacity);
    if (!buf.data) {
        buf.size = 0;
        buf.capacity = 0;
        return buf;
    }
    buf.size = 0;
    return buf;
}

void destroy_buffer(Buffer* buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size && output_size < output_capacity - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[output_size++] = current;
            output[output_size++] = (uint8_t)count;
            i += count;
        } else {
            output[output_size++] = current;
            i++;
        }
    }
    
    return output_size;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer compressed = create_buffer(CHUNK_SIZE);
    Buffer output = create_buffer(CHUNK_SIZE);
    
    if (!compressed.data || !output.data) {
        fprintf(stderr, "Memory allocation failed\n");
        destroy_buffer(&compressed);
        destroy_buffer(&output);
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes, Ctrl+D to end):\n", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        if (total_read < CHUNK_SIZE) {
            stack_input[total_read] = (uint8_t)c;
        }
        
        if (!buffer_append(&output, (uint8_t*)&c, 1)) {
            fprintf(stderr, "Buffer append failed\n");
            destroy_buffer(&compressed);
            destroy_buffer(&output);
            return 1;
        }
        total_read++;
    }
    
    if (total_read == 0) {
        printf("No input data\n");
        destroy_buffer(&compressed);
        destroy_buffer(&output);
        return 0;
    }
    
    uint8_t* compress_buf = malloc(total_read * 2);
    if (!compress_buf) {
        fprintf(stderr, "Compression buffer allocation failed\n");
        destroy_buffer(&compressed);
        destroy_buffer(&output);
        return 1;
    }
    
    size_t compressed_size = simple_compress(output.data, output.size, compress_buf, total_read * 2);
    
    if (!buffer_append(&compressed, compress_buf, compressed_size)) {
        fprintf(stderr, "Failed to store compressed data\n");
        free(compress_buf);
        destroy_buffer(&compressed);
        destroy_buffer(&output);
        return 1;
    }
    
    free(compress_buf);
    
    printf("Original size: %zu bytes\n", output.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (output.size > 0) ? (100.0 * compressed.size / output.size) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) printf("...");
    printf("\n");
    
    destroy_buffer(&compressed);
    destroy_buffer(&output);
    return 0;
}