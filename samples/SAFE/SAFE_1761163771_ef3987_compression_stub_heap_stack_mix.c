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
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (!buf->data) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!data || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) {
        return;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) {
            return;
        }
        i += count;
    }
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer heap_output;
    
    if (!buffer_init(&heap_output, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        size_t to_read = CHUNK_SIZE;
        if (total_read + to_read > MAX_INPUT_SIZE) {
            to_read = MAX_INPUT_SIZE - total_read;
        }
        
        size_t bytes_read = fread(stack_input, 1, to_read, stdin);
        if (bytes_read == 0) {
            break;
        }
        
        if (!buffer_append(&heap_output, stack_input, bytes_read)) {
            fprintf(stderr, "Failed to buffer input\n");
            buffer_free(&heap_output);
            return 1;
        }
        total_read += bytes_read;
        
        if (bytes_read < to_read) {
            break;
        }
    }
    
    if (heap_output.size == 0) {
        printf("No input data received\n");
        buffer_free(&heap_output);
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        buffer_free(&heap_output);
        return 1;
    }
    
    compress_rle(heap_output.data, heap_output.size, &compressed);
    
    printf("Original size: %zu bytes\n", heap_output.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed.size * 100.0) / heap_output.size);
    
    printf("Compressed data (first 100 bytes): ");
    for (size_t i = 0; i < compressed.size && i < 100; i += 2) {
        if (i + 1 < compressed.size) {
            printf("[%02X:%u] ", compressed.data[i], compressed.data[i + 1]);
        }
    }
    printf("\n");
    
    buffer_free(&heap_output);
    buffer_free(&compressed);
    return 0;
}