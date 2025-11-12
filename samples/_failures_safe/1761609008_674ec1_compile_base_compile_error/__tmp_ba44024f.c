//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
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

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr++ = (unsigned char)count;
        *out_ptr++ = value;
        
        current = next;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current + 1 < end) {
        unsigned char count = *current++;
        unsigned char value = *current++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (size_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_buf.size < input_buf.capacity - 1) {
        buffer_append(&input_buf, (unsigned char)c);
    }
    buffer_append(&input_buf, '\0');
    
    if (input_buf.size == 1) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    input_buf.size--;
    
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
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_buf.size / input_buf.size