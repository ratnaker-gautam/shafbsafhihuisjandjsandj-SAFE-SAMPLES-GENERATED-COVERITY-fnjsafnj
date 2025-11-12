//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
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
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0 || data_size > SIZE_MAX - buf->size) {
        return -1;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return -1;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return -1;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 0;
}

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (out_idx + 2 > CHUNK_SIZE) {
            break;
        }
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    *output_size = out_idx;
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    Buffer compressed_buffer;
    
    if (buffer_init(&compressed_buffer, CHUNK_SIZE * 2) != 0) {
        fprintf(stderr, "Failed to initialize buffer\n");
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_stack, sizeof(input_stack), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    size_t input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        uint8_t output_chunk[CHUNK_SIZE * 2];
        size_t compressed_size;
        
        compress_chunk((const uint8_t*)(input_stack + i), chunk_size, output_chunk, &compressed_size);
        
        if (compressed_size > 0) {
            if (buffer_append(&compressed_buffer, output_chunk, compressed_size) != 0) {
                fprintf(stderr, "Failed to append compressed data\n");
                buffer_free(&compressed_buffer);
                return 1;
            }
        }
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed_buffer.size * 100.0) / (input_len > 0 ? input_len : 1));
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_buffer.size; i++) {
        printf("%02x", compressed_buffer.data[i]);
    }
    printf("\n");
    
    buffer_free(&compressed_buffer);
    return 0;
}