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
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(EXIT_FAILURE);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
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
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end && output_size + 2 <= output_capacity) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        *(output + output_size) = (uint8_t)count;
        *(output + output_size + 1) = value;
        output_size += 2;
        
        current = next;
    }
    
    return output_size;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current + 1 < end && output_size < output_capacity) {
        uint8_t count = *current;
        uint8_t value = *(current + 1);
        
        if (output_size + count > output_capacity) break;
        
        for (size_t i = 0; i < count; i++) {
            *(output + output_size) = value;
            output_size++;
        }
        
        current += 2;
    }
    
    return output_size;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (!fgets(temp_input, sizeof(temp_input), stdin)) {
        printf("Input error\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        buffer_append(&input_buf, (uint8_t)temp_input[i]);
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_buf.data, compressed_buf.capacity);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    compressed_buf.size = compressed_size;
    
    size_t decompressed_size = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed_buf.data, decompressed_buf.capacity);
    
    if (decompressed_size != input_buf.size || memcmp(input_buf.data, decompressed_buf.data, input_buf.size) != 0) {
        printf("Decompression verification failed\n");
        buffer_free(&input