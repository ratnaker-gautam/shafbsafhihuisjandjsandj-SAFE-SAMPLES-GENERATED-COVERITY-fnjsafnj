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
    if (new_data == NULL) {
        return 0;
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    if (buf->size > new_capacity) {
        buf->size = new_capacity;
    }
    return 1;
}

int append_to_buffer(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - data_size) {
        return 0;
    }
    size_t required = buf->size + data_size;
    if (required > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < required) {
            new_capacity = required;
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

void free_buffer(Buffer* buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size && output_size < output_capacity - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        if (output_size + 2 > output_capacity) {
            break;
        }
        output[output_size++] = (uint8_t)count;
        output[output_size++] = current;
        i += count;
    }
    return output_size;
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_stack, sizeof(input_stack), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer input_buf = create_buffer(input_len + 1);
    if (input_buf.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!append_to_buffer(&input_buf, (const uint8_t*)input_stack, input_len)) {
        fprintf(stderr, "Buffer append failed\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    size_t max_output_size = input_len * 2 + 1;
    if (max_output_size > SIZE_MAX / 2) {
        fprintf(stderr, "Input too large\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    uint8_t* output_heap = malloc(max_output_size);
    if (output_heap == NULL) {
        fprintf(stderr, "Output buffer allocation failed\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, output_heap, max_output_size);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", compressed_size * 100.0 / input_len);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02x ", output_heap[i]);
    }
    if