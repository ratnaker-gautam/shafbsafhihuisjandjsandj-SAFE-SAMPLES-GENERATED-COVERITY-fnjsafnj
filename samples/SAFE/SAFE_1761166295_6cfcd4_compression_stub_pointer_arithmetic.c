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

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < UCHAR_MAX) {
            count++;
            next++;
        }
        if (out_ptr + 2 > out_end) {
            return 0;
        }
        *out_ptr++ = (unsigned char)count;
        *out_ptr++ = current;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_len = 0;
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input[input_len++] = (unsigned char)c;
    }
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    unsigned char compressed[MAX_OUTPUT_SIZE];
    size_t compressed_len = compress_rle(input, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100.0);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    Buffer decompressed;
    if (!buffer_init(&decompressed, input_len)) {
        printf("Failed to initialize buffer.\n");
        return 1;
    }
    const unsigned char *comp_ptr = compressed;
    const unsigned char *comp_end = compressed + compressed_len;
    while (comp_ptr < comp_end) {
        if (comp_ptr + 2 > comp_end) {
            printf("Invalid compressed data.\n");
            buffer_free(&decompressed);
            return 1;
        }
        unsigned char count = *comp_ptr++;
        unsigned char value = *comp_ptr++;
        for (unsigned char i = 0; i < count; i++) {
            if (!buffer_append(&decompressed, &value, 1)) {
                printf("Decompression buffer overflow.\n");
                buffer_free(&decompressed);
                return 1;
            }
        }
    }
    if (decompressed.size != input_len || memcmp(decompressed.data, input, input_len) != 0) {
        printf("Decompression verification failed.\n");
        buffer_free(&decompressed);
        return 1;
    }
    printf("Decompression verified successfully.\n");
    buffer_free(&decompressed);
    return 0;
}