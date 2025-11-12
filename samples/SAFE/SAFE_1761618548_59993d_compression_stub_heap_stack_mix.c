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
    if (buf == NULL || data == NULL || data_size == 0) {
        return -1;
    }
    
    if (buf->size > SIZE_MAX - data_size) {
        return -1;
    }
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > SIZE_MAX / 2) {
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

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) {
        return;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < UINT8_MAX) {
            count++;
            i++;
        }
        
        uint8_t run_length[2] = {current, (uint8_t)count};
        if (buffer_append(output, run_length, 2) != 0) {
            return;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed;
    if (buffer_init(&compressed, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    
    simple_compress((uint8_t*)input, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
        if (i < compressed.size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}