//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (src == NULL || len == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE || output_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    size_t out_index = 0;
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        if (out_index + 2 > output_capacity) {
            return 0;
        }
        *(output + out_index) = (unsigned char)count;
        *(output + out_index + 1) = value;
        out_index += 2;
        current += count;
    }
    return out_index;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_len = 0;
    printf("Enter input data (up to %zu bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        *(input + input_len) = (unsigned char)c;
        input_len++;
    }
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    unsigned char compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed or input too large.\n");
        return 1;
    }
    printf("Original size: %zu\n", input_len);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", (double)compressed_size / input_len);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", *(compressed + i));
    }
    printf("\n");
    Buffer decompressed;
    if (!buffer_init(&decompressed, input_len)) {
        printf("Failed to initialize decompression buffer.\n");
        return 1;
    }
    const unsigned char *comp_ptr = compressed;
    const unsigned char *comp_end = compressed + compressed_size;
    while (comp_ptr < comp_end) {
        if (comp_ptr + 1 >= comp_end) {
            printf("Invalid compressed data format.\n");
            buffer_free(&decompressed);
            return 1;
        }
        unsigned char count = *comp_ptr;
        unsigned char value = *(comp_ptr + 1);
        if (count == 0) {
            printf("Invalid run length in compressed data.\n");
            buffer_free(&decompressed);
            return 1;
        }
        for (unsigned char i = 0; i < count; i++) {
            if (!buffer_append(&decompressed, &value, 1)) {
                printf("Decompression buffer overflow.\n");
                buffer_free(&decompressed);