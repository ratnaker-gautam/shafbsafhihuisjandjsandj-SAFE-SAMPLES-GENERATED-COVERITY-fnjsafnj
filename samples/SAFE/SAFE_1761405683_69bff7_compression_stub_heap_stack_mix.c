//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.data = malloc(initial_capacity);
    if (buf.data == NULL) {
        buf.size = 0;
        buf.capacity = 0;
        return buf;
    }
    buf.size = 0;
    buf.capacity = initial_capacity;
    return buf;
}

void destroy_buffer(Buffer* buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (buf->data == NULL || data == NULL) {
        return 0;
    }
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
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

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) {
            return;
        }
        
        i += count;
    }
}

int read_input_data(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) {
        return 0;
    }
    
    size_t total_read = 0;
    
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF) {
            break;
        }
        
        if (total_read < max_size) {
            buffer[total_read] = (uint8_t)c;
            total_read++;
        }
    }
    
    *actual_size = total_read;
    return 1;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (!read_input_data(input_stack, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input data provided\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(CHUNK_SIZE);
    if (compressed.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress(input_stack, input_size, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        destroy_buffer(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 64; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 64) {
        printf("...");
    }
    printf("\n");
    
    destroy_buffer(&compressed);
    return 0;
}