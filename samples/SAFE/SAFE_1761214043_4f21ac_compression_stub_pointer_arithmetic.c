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

void buffer_append(Buffer *buf, const unsigned char *src, size_t count) {
    if (src == NULL || count == 0 || count > MAX_INPUT_SIZE) {
        return;
    }
    if (buf->size + count > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + count) {
            new_capacity = buf->size + count;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_end = src + count;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size += count;
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
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (unsigned char)count;
        out_ptr++;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    char input_line[MAX_INPUT_SIZE + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len > MAX_INPUT_SIZE) {
        buffer_free(&input_buf);
        printf("Input too long.\n");
        return EXIT_FAILURE;
    }
    buffer_append(&input_buf, (unsigned char*)input_line, input_len);
    unsigned char output[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output, sizeof(output));
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", compressed_size * 100.0 / input_buf.size);
    printf("Compressed data: ");
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + compressed_size;
    while (out_ptr < out_end) {
        printf("(%c,%d) ", *out_ptr, *(out_ptr + 1));
        out_ptr += 2;
    }
    printf("\n");
    buffer_free(&input_buf);
    return EXIT_SUCCESS;
}