//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
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

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *end = src + len;
    while (src < end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size += len;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
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
        if (out_ptr + 2 > out_end) {
            return 0;
        }
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (uint8_t)count;
        out_ptr++;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf;
    if (!buffer_init(&input_buf, 1024)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    uint8_t temp_buf[MAX_INPUT_SIZE];
    if (fgets((char*)temp_buf, sizeof(temp_buf), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        return 1;
    }
    size_t input_len = strlen((char*)temp_buf);
    if (input_len > 0 && temp_buf[input_len - 1] == '\n') {
        temp_buf[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Input is empty\n");
        buffer_free(&input_buf);
        return 1;
    }
    if (!buffer_append(&input_buf, temp_buf, input_len)) {
        fprintf(stderr, "Failed to store input\n");
        buffer_free(&input_buf);
        return 1;
    }
    uint8_t output_buf[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf, sizeof(output_buf));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);
    printf("Compressed data: ");
    const uint8_t *out_ptr = output_buf;
    const uint8_t *out_end = output_buf + compressed_size;
    while (out_ptr < out_end) {
        printf("0x%02X(%d) ", *out_ptr, *(out_ptr + 1));
        out_ptr += 2;
    }
    printf("\n");
    buffer_free(&input_buf);
    return 0;
}