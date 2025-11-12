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
        if (new_capacity > MAX_OUTPUT_SIZE) new_capacity = MAX_OUTPUT_SIZE;
        if (new_capacity <= buf->capacity) return;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return;
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
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) break;
        
        *out_ptr++ = value;
        *out_ptr++ = (uint8_t)count;
        
        current = next;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (current + 1 < end) {
        uint8_t value = *current++;
        uint8_t count = *current++;
        
        if (out_ptr + count > out_end) break;
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    return out_ptr - output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input_data);
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_size = compress_rle(input_data, input_len, compressed, MAX_OUTPUT_SIZE);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size > 0) {
        printf("Compression ratio: %.2f%%\n", (float)compressed_size / input_len * 100);
        
        size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
        decompressed[decompressed_size] = '\0';
        
        printf("Decompressed: %s\n", decompressed);
        printf("Decompressed size: %zu bytes\n", decompressed_size);
        
        if (decompressed_size == input_len && memcmp(input_data, decompressed, input_len) == 0) {
            printf("Verification: SUCCESS\n");
        } else {
            printf("Verification: FAILED\n");
        }
    } else {
        printf("Compression failed\n");
    }
    
    Buffer test_buffer;
    buffer_init(&test_buffer, 10);
    
    for (int i = 0; i < 5; i++) {
        buffer