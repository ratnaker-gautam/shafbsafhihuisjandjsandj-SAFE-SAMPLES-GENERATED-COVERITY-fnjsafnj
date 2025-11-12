//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (!input || !output || !output_size || input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count >= 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            output[out_idx++] = current;
            i++;
        }
        
        if (out_idx >= CHUNK_SIZE - 3) {
            break;
        }
    }
    *output_size = out_idx;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer input_buf = create_buffer(input_len);
    if (!input_buf.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    memcpy(input_buf.data, input_str, input_len);
    input_buf.size = input_len;
    
    Buffer output_buf = create_buffer(input_len * 2);
    if (!output_buf.data) {
        fprintf(stderr, "Memory allocation failed\n");
        destroy_buffer(&input_buf);
        return 1;
    }
    
    uint8_t chunk_buffer[CHUNK_SIZE];
    size_t chunk_output_size;
    
    for (size_t i = 0; i < input_buf.size; ) {
        size_t chunk_size = (input_buf.size - i < CHUNK_SIZE) ? input_buf.size - i : CHUNK_SIZE;
        
        compress_chunk(input_buf.data + i, chunk_size, chunk_buffer, &chunk_output_size);
        
        if (!buffer_append(&output_buf, chunk_buffer, chunk_output_size)) {
            fprintf(stderr, "Compression failed\n");
            destroy_buffer(&input_buf);
            destroy_buffer(&output_buf);
            return 1;
        }
        
        i += chunk_size;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", output_buf.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_buf.size > 0) ? (100.0 * output_buf.size / input_buf.size) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_buf.size; i++) {
        printf("%02X", output_buf.data[i]);
    }
    printf("\n");
    
    destroy_buffer(&input_buf);
    destroy_buffer(&output_buf);
    
    return 0;
}