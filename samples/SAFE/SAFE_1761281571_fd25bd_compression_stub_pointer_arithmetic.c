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
    if (buf->size > SIZE_MAX - len) {
        exit(EXIT_FAILURE);
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                exit(EXIT_FAILURE);
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            exit(EXIT_FAILURE);
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_end = src + len;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size = new_size;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
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
            return 0;
        }
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (unsigned char)count;
        out_ptr++;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    char input_line[MAX_INPUT_SIZE + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    Buffer compressed;
    buffer_init(&compressed, 64);
    unsigned char output_buf[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle((unsigned char*)input_line, input_len, output_buf, sizeof(output_buf));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    buffer_append(&compressed, output_buf, compressed_size);
    printf("Original size: %zu\n", input_len);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    const unsigned char *ptr = compressed.data;
    const unsigned char *end = ptr + compressed.size;
    while (ptr < end) {
        printf("%02X ", *ptr);
        ptr++;
    }
    printf("\n");
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}