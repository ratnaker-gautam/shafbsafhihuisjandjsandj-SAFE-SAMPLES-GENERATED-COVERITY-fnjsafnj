//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
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
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        uint8_t run[2] = {value, (uint8_t)count};
        if (!buffer_append(output, run, 2)) return 0;
        
        current = next;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (count == 0) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
        
        current += 2;
    }
    
    return 1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&compressed_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    if (!buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return 1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && bytes_read < MAX_INPUT_SIZE) {
        input_buf.data[bytes_read] = (uint8_t)c;
        bytes_read++;
    }
    
    if (bytes_read == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    input_buf.size = bytes_read;
    
    if (!compress_rle(input_buf.data, input_buf.size, &compressed_buf)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    if (!decompress_rle(compressed_buf.data, compressed_buf.size, &decompressed_buf)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size