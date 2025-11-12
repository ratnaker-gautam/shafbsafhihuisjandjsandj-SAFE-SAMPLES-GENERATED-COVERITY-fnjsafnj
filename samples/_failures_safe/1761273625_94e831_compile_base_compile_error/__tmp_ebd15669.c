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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
        return -1;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return -1;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf->data == NULL || data == NULL) {
        return -1;
    }
    if (len > SIZE_MAX - buf->size) {
        return -1;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            return -1;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return -1;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return -1;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 0;
}

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_len = 0;
    for (size_t i = 0; i < input_len && output_len < output_capacity - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        if (count >= 3) {
            if (output_len + 2 > output_capacity) {
                break;
            }
            output[output_len++] = current;
            output[output_len++] = (uint8_t)count;
            i += count;
        } else {
            if (output_len + 1 > output_capacity) {
                break;
            }
            output[output_len++] = current;
            i++;
        }
    }
    return output_len;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer heap_buffer;
    Buffer compressed_buffer;
    
    if (buffer_init(&heap_buffer, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (buffer_init(&compressed_buffer, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&heap_buffer);
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes, Ctrl+D to end):\n", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        size_t bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) {
                break;
            } else {
                fprintf(stderr, "Error reading input\n");
                buffer_free(&heap_buffer);
                buffer_free(&compressed_buffer);
                return 1;
            }
        }
        
        if (buffer_append(&heap_buffer, stack_input, bytes_read) != 0) {
            fprintf(stderr, "Input too large or memory error\n");
            buffer_free(&heap_buffer);
            buffer_free(&compressed_buffer);
            return 1;
        }
        
        total_read += bytes_read;
        if (bytes_read < CHUNK_SIZE) {
            break;
        }
    }
    
    if (heap_buffer.size == 0) {
        printf("No input data provided\n");
        buffer_free(&heap_buffer);
        buffer_free(&compressed_buffer);
        return 0;
    }
    
    uint8_t *compressed_data = malloc(heap_buffer.size * 2);
    if (compressed_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&heap_buffer);
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    size_t compressed_size = simple_compress(heap_buffer.data, heap_buffer.size, compressed_data, heap_buffer.size * 2);
    
    if (compressed_size > 0) {
        if (buffer_append(&compressed_buffer, compressed_data, compressed_size) != 0) {
            fprintf(stderr, "Failed to store compressed data\n");
            free(compressed_data);
            buffer_free(&heap_buffer);
            buffer_free(&compressed_buffer