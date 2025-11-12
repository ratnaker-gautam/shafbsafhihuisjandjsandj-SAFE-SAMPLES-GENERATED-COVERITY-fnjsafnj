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
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) new_capacity *= 2;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        const unsigned char *next = in_ptr + 1;
        
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr++ = current;
        *out_ptr++ = (unsigned char)count;
        
        in_ptr += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr + 1 < in_end) {
        unsigned char value = *in_ptr++;
        unsigned char count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (unsigned char i = 0; i < count; i++) {
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
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    unsigned char temp_buf[MAX_INPUT_SIZE];
    if (fgets((char*)temp_buf, sizeof(temp_buf), stdin) == NULL) {
        printf("Error reading input\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen((char*)temp_buf);
    if (input_len > 0 && temp_buf[input_len - 1] == '\n') {
        temp_buf[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    buffer_append(&input_buf, temp_buf, input_len);
    
    unsigned char comp_output[MAX_OUTPUT_SIZE];
    size_t comp_size = compress_rle(input_buf.data, input_buf.size, comp_output, sizeof(comp_output));
    
    if (comp_size == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    buffer_append(&compressed_buf, comp_output, comp_size);
    
    unsigned char decomp_output[MAX_INPUT_SIZE];
    size_t decomp_size = decompress_rle(compressed_buf.data, compressed_buf.size, decomp_output, sizeof(decomp_output));
    
    if (decomp_size == 0) {
        printf("Decompression failed\n");
        buffer_free(&input_buf);
        buffer_free