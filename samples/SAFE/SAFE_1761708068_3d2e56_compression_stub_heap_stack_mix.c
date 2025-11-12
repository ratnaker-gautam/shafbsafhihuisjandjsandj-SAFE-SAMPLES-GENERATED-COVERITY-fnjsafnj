//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= SIZE_MAX / 2) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int resize_buffer(Buffer* buf, size_t new_capacity) {
    if (new_capacity == 0 || new_capacity > SIZE_MAX / 2) {
        return 0;
    }
    uint8_t* new_data = realloc(buf->data, new_capacity);
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

void free_buffer(Buffer* buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int append_to_buffer(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (!buf || !data || data_size == 0 || data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return 0;
        }
        if (!resize_buffer(buf, new_capacity)) {
            return 0;
        }
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!input || input_size == 0 || !output) {
        return;
    }
    
    size_t stack_buffer[CHUNK_SIZE];
    size_t stack_index = 0;
    
    for (size_t i = 0; i < input_size; i++) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        
        if (stack_index < CHUNK_SIZE) {
            stack_buffer[stack_index++] = (size_t)current | (count << 8);
        }
        
        if (stack_index >= CHUNK_SIZE || i == input_size - 1) {
            for (size_t j = 0; j < stack_index; j++) {
                uint8_t byte = stack_buffer[j] & 0xFF;
                uint8_t run_length = (stack_buffer[j] >> 8) & 0xFF;
                
                uint8_t chunk[2] = {run_length, byte};
                if (!append_to_buffer(output, chunk, 2)) {
                    return;
                }
            }
            stack_index = 0;
        }
    }
}

int main() {
    char user_input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(user_input, sizeof(user_input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(64);
    if (!compressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress((uint8_t*)user_input, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
        if ((i + 1) % 16 == 0 && i + 1 < compressed.size) {
            printf("\n");
        }
    }
    printf("\n");
    
    free_buffer(&compressed);
    return 0;
}