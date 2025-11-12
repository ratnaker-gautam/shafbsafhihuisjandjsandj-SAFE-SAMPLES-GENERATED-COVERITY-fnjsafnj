//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    buf->data = malloc(initial_capacity);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf->data == NULL || data == NULL) {
        return 0;
    }
    if (len == 0) {
        return 1;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_len = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    while (current < end && output_len + 2 <= output_capacity) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        *(output + output_len) = value;
        *(output + output_len + 1) = (uint8_t)count;
        output_len += 2;
        current = next;
    }
    return output_len;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0 || input_len % 2 != 0) {
        return 0;
    }
    size_t output_len = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    while (current < end && output_len < output_capacity) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        if (output_len + count > output_capacity) {
            return 0;
        }
        for (uint8_t i = 0; i < count; i++) {
            *(output + output_len) = value;
            output_len++;
        }
        current += 2;
    }
    return output_len;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    if (input_buf.data == NULL) {
        return 1;
    }
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        buffer_free(&input_buf);
        return 1;
    }
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        buffer_free(&input_buf);
        return 1;
    }
    if (!buffer_append(&input_buf, (uint8_t*)line, input_len)) {
        buffer_free(&input_buf);
        return 1;
    }
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_len = compress_rle(input_buf.data, input_buf.size, compressed, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", *(compressed + i));
    }
    printf("\n");
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_len = decompress_rle(compressed, compressed_len, decompressed, MAX_INPUT_SIZE);
    if (decompressed_len == 0 || decompressed_len != input_buf.size) {
        printf("Decompression failed\n");
        buffer_free(&input_buf);
        return 1