//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t capacity) {
    if (buf == NULL || capacity == 0 || capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        return 0;
    }
    uint8_t *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += len;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        uint8_t run[2] = {value, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        current += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return 0;
    }
    if (input_len % 2 != 0 || input_len > MAX_OUTPUT_SIZE) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        if (count == 0) {
            return 0;
        }
        for (uint8_t i = 0; i < count; i++) {
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
        }
        current += 2;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_data[input_len++] = (uint8_t)c;
    }
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, MAX_OUTPUT_SIZE)) {
        printf("Failed to initialize compression buffer.\n");
        return 1;
    }
    if (!compress_rle(input_data, input_len, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Compressed size: %zu bytes\n", compressed.size);
    Buffer decompressed;
    if (!buffer_init(&decompressed, MAX_OUTPUT_SIZE)) {
        printf("Failed to initialize decompression buffer.\n");
        buffer_free(&compressed);
        return 1;
    }
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    if (decompressed.size != input_len || memcmp(decompressed.data, input_data, input_len) != 0) {
        printf("Decompression verification failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    printf("Decompression successful. Original data verified.\n");
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return 0;
}