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
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
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
    
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    
    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *scan = in_ptr + 1;
        while (scan < in_end && *scan == current && count < UCHAR_MAX) {
            count++;
            scan++;
        }
        
        if (count >= 3) {
            *out_ptr++ = current;
            *out_ptr++ = (unsigned char)count;
            in_ptr += count;
        } else {
            *out_ptr++ = current;
            in_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, output_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&output_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    unsigned char temp_buf[MAX_INPUT_SIZE];
    size_t bytes_read = 0;
    
    int c;
    while ((c = getchar()) != EOF && bytes_read < MAX_INPUT_SIZE - 1) {
        if (c == '\n') break;
        temp_buf[bytes_read++] = (unsigned char)c;
    }
    
    if (bytes_read == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 0;
    }
    
    if (!buffer_append(&input_buf, temp_buf, bytes_read)) {
        fprintf(stderr, "Error: Failed to store input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf.data, output_buf.capacity);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", output_buf.data[i]);
    }
    printf("\n");
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    return 0;
}