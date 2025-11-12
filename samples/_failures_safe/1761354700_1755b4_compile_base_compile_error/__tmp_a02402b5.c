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
    if (!buf->data) exit(1);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(1);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(1);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        
        in_ptr += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0 || input_size % 2 != 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    return out_ptr - output;
}

int main() {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (!fgets(temp_input, sizeof(temp_input), stdin)) {
        printf("Input error\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
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
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        buffer_append(&input_buf, (uint8_t)temp_input[i]);
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    for (size_t i = 0; i < compressed_size; i++) {
        buffer_append(&compressed_buf, compressed[i]);
    }
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed, sizeof(decompressed));