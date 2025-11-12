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

void buffer_append(Buffer *buf, const uint8_t *src, size_t count) {
    if (src == NULL || count == 0 || count > MAX_OUTPUT_SIZE) {
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
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, count);
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

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }

    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;

    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }

        if (out_ptr + 2 > out_end) {
            break;
        }
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        in_ptr += count;
    }

    return out_ptr - output;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, 256);

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }

    size_t line_len = strlen(line);
    if (line_len > 0 && line[line_len - 1] == '\n') {
        line[line_len - 1] = '\0';
        line_len--;
    }

    if (line_len == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }

    if (line_len > MAX_INPUT_SIZE) {
        printf("Input too large.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }

    buffer_append(&input_buf, (uint8_t*)line, line_len);

    uint8_t output[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output, sizeof(output));

    if (compressed_size == 0) {
        printf("Compression failed or no data compressed.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);

    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02x ", output[i]);
    }
    printf("\n");

    buffer_free(&input_buf);
    return EXIT_SUCCESS;
}