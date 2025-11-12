//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / 2) return 0;
    
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

int buffer_append(Buffer *buf, const unsigned char *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src = data;
    size_t i = 0;
    while (i < data_size) {
        *(dest + i) = *(src + i);
        i++;
    }
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_size + 2 > output_capacity) return 0;
        
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        *(output + output_size) = (unsigned char)count;
        *(output + output_size + 1) = current;
        output_size += 2;
        i += count;
    }
    
    return output_size;
}

int main(void) {
    Buffer input_buf, output_buf;
    
    if (!buffer_init(&input_buf, 64)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&output_buf, 128)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    unsigned char temp_buf[MAX_INPUT_SIZE + 1];
    if (fgets((char*)temp_buf, sizeof(temp_buf), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t input_len = strlen((char*)temp_buf);
    if (input_len > 0 && temp_buf[input_len - 1] == '\n') {
        temp_buf[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    if (!buffer_append(&input_buf, temp_buf, input_len)) {
        fprintf(stderr, "Failed to store input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    unsigned char compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {