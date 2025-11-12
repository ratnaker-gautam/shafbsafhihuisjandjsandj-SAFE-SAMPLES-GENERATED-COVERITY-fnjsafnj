//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return -1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return -1;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return -1;
    if (buf->size > SIZE_MAX - len) return -1;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return -1;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
    return 0;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        if (buffer_append(output, &count, 1) != 0) return -1;
        if (buffer_append(output, &current, 1) != 0) return -1;
        i += count;
    }
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return -1;
    size_t i = 0;
    while (i < input_len) {
        uint8_t count = *(input + i);
        uint8_t value = *(input + i + 1);
        if (count == 0) return -1;
        for (uint8_t j = 0; j < count; j++) {
            if (buffer_append(output, &value, 1) != 0) return -1;
        }
        i += 2;
    }
    return 0;
}

int main(void) {
    uint8_t input_buffer[BUFFER_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (buffer_init(&compressed, BUFFER_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    
    if (buffer_init(&decompressed, BUFFER_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", BUFFER_SIZE - 1);
    if (fgets((char*)input_buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (compress_rle(input_buffer, input_len, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;