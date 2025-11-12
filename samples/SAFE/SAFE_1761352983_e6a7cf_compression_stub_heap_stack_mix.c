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

Buffer buffer_create(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= SIZE_MAX / 2) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_resize(Buffer *buf, size_t new_capacity) {
    if (new_capacity == 0 || new_capacity > SIZE_MAX / 2) {
        return 0;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    if (buf->size > buf->capacity) {
        buf->size = buf->capacity;
    }
    return 1;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
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
        if (!buffer_resize(buf, new_capacity)) {
            return 0;
        }
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (!input || !output || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        if (count >= 3) {
            if (output_size + 2 > CHUNK_SIZE * 2) {
                return 0;
            }
            output[output_size++] = current;
            output[output_size++] = (uint8_t)count;
            i += count;
        } else {
            if (output_size + count > CHUNK_SIZE * 2) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_size++] = input[i++];
            }
        }
    }
    return output_size;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t total_read = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (total_read < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_stack[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed = buffer_create(CHUNK_SIZE * 2);
    if (!compressed.data) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < total_read) {
        size_t chunk_size = total_read - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        uint8_t output_stack[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_stack + processed, chunk_size, output_stack);
        
        if (compressed_size == 0 || !buffer_append(&compressed, output_stack, compressed_size)) {
            printf("Compression failed.\n");
            buffer_free(&compressed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    printf("Original size: %zu bytes\n", total_read);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / total_read) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}