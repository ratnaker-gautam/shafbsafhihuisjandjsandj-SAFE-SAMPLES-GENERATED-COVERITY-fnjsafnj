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
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (buf->size + len > new_capacity) {
            new_capacity = buf->size + len;
        }
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
    size_t i = 0;
    while (i < input_len) {
        if (output_len + 2 > output_capacity) return 0;
        unsigned char count = 1;
        while (i + count < input_len && count < UCHAR_MAX && *(input + i) == *(input + i + count)) {
            count++;
        }
        *(output + output_len) = count;
        *(output + output_len + 1) = *(input + i);
        output_len += 2;
        i += count;
    }
    return output_len;
}

int main(void) {
    Buffer input_buf, output_buf;
    if (!buffer_init(&input_buf, BUFFER_SIZE) || !buffer_init(&output_buf, BUFFER_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter data to compress (max %d bytes):\n", BUFFER_SIZE);
    unsigned char temp[BUFFER_SIZE];
    if (fgets((char*)temp, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }

    size_t input_len = strlen((char*)temp);
    if (input_len > 0 && temp[input_len - 1] == '\n') {
        temp[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }

    if (!buffer_append(&input_buf, temp, input_len)) {
        fprintf(stderr, "Buffer append failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }

    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf.data, output_buf.capacity);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    output_buf.size = compressed_size;

    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", output_buf.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)output_buf.size / input_buf.size) * 100);

    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_buf.size; i++) {
        printf("%02X ", *(output_buf.data + i));
    }
    printf("\n");

    buffer_free(&input_buf);
    buffer_free(&output_buf);
    return 0;
}