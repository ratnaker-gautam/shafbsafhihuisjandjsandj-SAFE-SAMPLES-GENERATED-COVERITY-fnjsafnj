//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    buf->data = malloc(initial_capacity);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

void compress_run_length(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input_len == 0) {
        return;
    }
    
    uint8_t current = *input;
    size_t count = 1;
    
    for (size_t i = 1; i < input_len; i++) {
        if (*(input + i) == current && count < 255) {
            count++;
        } else {
            uint8_t pair[2] = {current, (uint8_t)count};
            if (!buffer_append(output, pair, 2)) {
                return;
            }
            current = *(input + i);
            count = 1;
        }
    }
    
    uint8_t pair[2] = {current, (uint8_t)count};
    buffer_append(output, pair, 2);
}

void decompress_run_length(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input_len % 2 != 0) {
        return;
    }
    
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t value = *(input + i);
        uint8_t count = *(input + i + 1);
        
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) {
                return;
            }
        }
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed, decompressed;
    
    buffer_init(&compressed, 64);
    buffer_init(&decompressed, 64);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    compress_run_length(input_data, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input_data, input_len) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(*(decompressed.data + i));
    }
    putchar('\n');
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}