//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity <= buf->capacity) exit(EXIT_FAILURE);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (output_size + 2 > output_capacity) return 0;
        
        *(output + output_size) = value;
        *(output + output_size + 1) = (uint8_t)count;
        output_size += 2;
        
        current = next;
    }
    
    return output_size;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t output_size = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (output_size + count > output_capacity) return 0;
        
        uint8_t *target = output + output_size;
        for (uint8_t i = 0; i < count; i++) {
            *(target + i) = value;
        }
        
        output_size += count;
        current += 2;
    }
    
    return output_size;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_buf.size < MAX_INPUT_SIZE) {
        buffer_append(&input_buf, (uint8_t)c);
    }
    
    if (input_buf.size == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_buf.data, compressed_buf.capacity);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    compressed_buf.size = compressed_size;
    
    size_t decompressed_size = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed_buf.data, decompressed_buf.capacity);
    if (decompressed_size == 0 || decompressed_size != input_buf.size) {
        printf("Decompression failed.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    decompressed_buf.size = decompressed_size;
    
    if (memcmp(input_buf.data, decompressed_buf.data, input_buf.size) != 0) {
        printf("Data mismatch after decompression.\n");
        buffer_free(&input_buf);