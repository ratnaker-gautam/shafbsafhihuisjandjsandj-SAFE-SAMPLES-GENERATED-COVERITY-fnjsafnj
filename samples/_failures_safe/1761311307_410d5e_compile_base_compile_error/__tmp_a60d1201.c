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

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0 || src == NULL) {
        return;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (buf->size + len > new_capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_size;
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
        if (out_ptr + 2 > out_end) {
            break;
        }
        *out_ptr++ = (unsigned char)count;
        *out_ptr++ = current;
        in_ptr += count;
    }
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    while (in_ptr + 1 < in_end) {
        unsigned char count = *in_ptr++;
        unsigned char value = *in_ptr++;
        if (out_ptr + count > out_end) {
            break;
        }
        for (unsigned char i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    return out_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char compressed[MAX_OUTPUT_SIZE];
    unsigned char decompressed[MAX_INPUT_SIZE];
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE - 1);
    if (fgets((char *)input, sizeof(input), stdin) == NULL) {
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    size_t input_len = strlen((char *)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    size_t compressed_size = compress_rle(input, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    printf("Compressed %zu bytes to %zu bytes\n", input_len, compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, sizeof(decomp