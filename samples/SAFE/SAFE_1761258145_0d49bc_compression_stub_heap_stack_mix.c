//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
    if (buf->data == NULL) return 0;
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run_length_encoding[2] = {0xFF, (uint8_t)count};
            if (!buffer_append(output, run_length_encoding, 2)) return;
            if (!buffer_append(output, &current, 1)) return;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (input[i + j] == 0xFF) {
                    uint8_t escape[2] = {0xFF, 0x01};
                    if (!buffer_append(output, escape, 2)) return;
                }
                if (!buffer_append(output, &input[i + j], 1)) return;
            }
            i += count;
        }
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    
    return total_read;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    Buffer output_buffer;
    
    if (!buffer_init(&output_buffer, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        return 1;
    }
    
    int input_size = read_input(input_stack, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "No input data received\n");
        buffer_free(&output_buffer);
        return 1;
    }
    
    printf("Original data (%d bytes):\n", input_size);
    print_hex(input_stack, input_size);
    
    simple_compress(input_stack, input_size, &output_buffer);
    
    printf("Compressed data (%zu bytes):\n", output_buffer.size);
    print_hex(output_buffer.data, output_buffer.size);
    
    double ratio = (input_size > 0) ? (double)output_buffer.size / input_size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    buffer_free(&output_buffer);
    return 0;
}