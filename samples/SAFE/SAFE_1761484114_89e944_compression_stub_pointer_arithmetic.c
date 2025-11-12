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
        while (new_capacity < buf->size + len) {
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
    const unsigned char *input_end = input + input_size;
    unsigned char *output_start = output;
    unsigned char *output_end = output + output_capacity;
    while (input < input_end) {
        unsigned char current = *input;
        size_t count = 1;
        const unsigned char *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (output + 2 > output_end) {
            break;
        }
        *output++ = (unsigned char)count;
        *output++ = current;
        input += count;
    }
    return output - output_start;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    printf("Enter data to compress (max %zu bytes): ", (size_t)MAX_INPUT_SIZE);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    if (len > MAX_INPUT_SIZE) {
        len = MAX_INPUT_SIZE;
    }
    memcpy(input_buf.data, line, len);
    input_buf.size = len;
    unsigned char output[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output, sizeof(output));
    printf("Original size: %zu\n", input_buf.size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", input_buf.size > 0 ? (float)compressed_size / input_buf.size : 0.0f);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", output[i]);
    }
    printf("\n");
    buffer_free(&input_buf);
    return EXIT_SUCCESS;
}