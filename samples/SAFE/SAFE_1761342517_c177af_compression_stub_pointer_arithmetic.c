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
    size_t output_len = 0;
    const unsigned char *end = input + input_len;
    const unsigned char *current = input;
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        if (output_len + 2 > output_capacity) return 0;
        *(output + output_len) = (unsigned char)count;
        *(output + output_len + 1) = value;
        output_len += 2;
        current = next;
    }
    return output_len;
}

int main(void) {
    Buffer input_buf;
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    printf("Enter data to compress (max %zu bytes): ", (size_t)MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_buf.size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&input_buf);
            return 1;
        }
        if (!buffer_append(&input_buf, (unsigned char*)&c, 1)) {
            fprintf(stderr, "Failed to append to buffer\n");
            buffer_free(&input_buf);
            return 1;
        }
    }
    if (input_buf.size == 0) {
        fprintf(stderr, "No input provided\n");
        buffer_free(&input_buf);
        return 1;
    }
    unsigned char output[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed or output buffer too small\n");
        buffer_free(&input_buf);
        return 1;
    }
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", *(output + i));
    }
    printf("\n");
    buffer_free(&input_buf);
    return 0;
}