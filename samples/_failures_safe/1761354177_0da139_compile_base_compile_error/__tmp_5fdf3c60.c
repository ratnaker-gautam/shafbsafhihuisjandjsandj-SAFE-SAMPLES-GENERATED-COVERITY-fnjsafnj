//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void init_buffer(ByteBuffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int resize_buffer(ByteBuffer *buf, size_t new_capacity) {
    if (new_capacity == 0) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
        return 1;
    }
    
    if (new_capacity <= buf->capacity) {
        return 1;
    }
    
    unsigned char *new_data = realloc(buf->data, new_capacity);
    if (new_data == NULL) {
        return 0;
    }
    
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

void free_buffer(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int append_byte(ByteBuffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity == 0 ? 16 : buf->capacity * 2;
        if (new_capacity <= buf->capacity || !resize_buffer(buf, new_capacity)) {
            return 0;
        }
    }
    
    buf->data[buf->size] = byte;
    buf->size++;
    return 1;
}

size_t compress_run_length_recursive(const unsigned char *input, size_t input_len, size_t pos, ByteBuffer *output) {
    if (pos >= input_len) {
        return pos;
    }
    
    unsigned char current = input[pos];
    size_t run_length = 1;
    
    while (pos + run_length < input_len && input[pos + run_length] == current && run_length < 255) {
        run_length++;
    }
    
    if (run_length >= 3) {
        if (!append_byte(output, current) || !append_byte(output, (unsigned char)run_length)) {
            return pos;
        }
    } else {
        for (size_t i = 0; i < run_length; i++) {
            if (!append_byte(output, current)) {
                return pos + i;
            }
        }
    }
    
    return compress_run_length_recursive(input, input_len, pos + run_length, output);
}

size_t decompress_run_length_recursive(const unsigned char *input, size_t input_len, size_t pos, ByteBuffer *output) {
    if (pos >= input_len) {
        return pos;
    }
    
    if (pos + 1 < input_len && input[pos + 1] >= 3) {
        unsigned char value = input[pos];
        unsigned char count = input[pos + 1];
        
        for (unsigned char i = 0; i < count; i++) {
            if (!append_byte(output, value)) {
                return pos;
            }
        }
        
        return decompress_run_length_recursive(input, input_len, pos + 2, output);
    } else {
        if (!append_byte(output, input[pos])) {
            return pos;
        }
        return decompress_run_length_recursive(input, input_len, pos + 1, output);
    }
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    ByteBuffer compressed, decompressed;
    size_t input_len;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    init_buffer(&compressed);
    init_buffer(&decompressed);
    
    size_t compressed_pos = compress_run_length_recursive(input, input_len, 0, &compressed);
    if (compressed_pos != input_len) {
        fprintf(stderr, "Compression failed\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    size_t decompressed_pos = decompress_run_length_recursive(compressed.data, compressed.size, 0, &decompressed);
    if (decompressed_pos != compressed.size) {
        fprintf(stderr, "Decompression failed\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    
    if (input_len == decompressed.size && memcmp(input, decompressed.data, input_len) == 0) {
        printf