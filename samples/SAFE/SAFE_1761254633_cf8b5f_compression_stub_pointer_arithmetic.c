//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

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
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
    return 1;
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

int main(void) {
    Buffer input_buf, output_buf;
    if (!buffer_init(&input_buf, BUFFER_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (!buffer_init(&output_buf, BUFFER_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    unsigned char temp_buf[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(temp_buf, 1, sizeof(temp_buf), stdin)) > 0) {
        if (!buffer_append(&input_buf, temp_buf, bytes_read)) {
            fprintf(stderr, "Failed to read input data\n");
            buffer_free(&input_buf);
            buffer_free(&output_buf);
            return 1;
        }
    }
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading from stdin\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size > SIZE_MAX / 2) {
        fprintf(stderr, "Input too large to process\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    if (!buffer_append(&output_buf, NULL, max_output_size)) {
        fprintf(stderr, "Failed to allocate output buffer\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    output_buf.size = 0;
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf.data, output_buf.capacity);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    output_buf.size = compressed_size;
    size_t written = fwrite(output_buf.data, 1, output_buf.size, stdout);
    if (written != output_buf.size) {
        fprintf(stderr, "Failed to write output data\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    return 0;
}