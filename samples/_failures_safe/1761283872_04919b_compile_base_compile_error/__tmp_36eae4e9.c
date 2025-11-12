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

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (buf->size + len > new_capacity) new_capacity = buf->size + len;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    if (input_len > SIZE_MAX / 2) return 0;
    
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    
    while (in_ptr < in_end) {
        if (out_ptr + 2 > out_end) return 0;
        unsigned char current = *in_ptr;
        size_t count = 1;
        const unsigned char *scan = in_ptr + 1;
        while (scan < in_end && *scan == current && count < 255) {
            count++;
            scan++;
        }
        *out_ptr++ = current;
        *out_ptr++ = (unsigned char)count;
        in_ptr += count;
    }
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    if (input_len % 2 != 0) return 0;
    
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    
    while (in_ptr < in_end) {
        if (in_ptr + 2 > in_end) return 0;
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
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_buf.data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    input_buf.size = strlen((char*)input_buf.data);
    if (input_buf.size > 0 && input_buf.data[input_buf.size - 1] == '\n') {
        input_buf.data[input_buf.size - 1] = '\0';
        input_buf.size--;
    }
    
    if (input_buf.size == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free