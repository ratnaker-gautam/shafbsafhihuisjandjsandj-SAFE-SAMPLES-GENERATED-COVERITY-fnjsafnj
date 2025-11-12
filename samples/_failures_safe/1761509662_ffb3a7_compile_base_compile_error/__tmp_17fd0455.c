//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return -1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return -1;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return -1;
    if (buf->size > SIZE_MAX - len) return -1;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return -1;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 0;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (buffer_append(output, run, 2) != 0) return -1;
        i += count;
    }
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return -1;
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t value = *(input + i);
        uint8_t count = *(input + i + 1);
        if (count == 0) return -1;
        for (uint8_t j = 0; j < count; j++) {
            if (buffer_append(output, &value, 1) != 0) return -1;
        }
    }
    return 0;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    if (buffer_init(&input_buf, BUFFER_SIZE) != 0) return EXIT_FAILURE;
    if (buffer_init(&compressed_buf, BUFFER_SIZE) != 0) {
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    if (buffer_init(&decompressed_buf, BUFFER_SIZE) != 0) {
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return EXIT_FAILURE;
    }

    printf("Enter text to compress (max %d chars): ", BUFFER_SIZE - 1);
    char temp[BUFFER_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        printf("Input error\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') temp[--len] = '\0';
    if (len == 0) {
        printf("Empty input\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    if (buffer_append(&input_buf, (uint8_t*)temp, len) != 0) {
        printf("Buffer append failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", temp);
    printf("Original size: %zu bytes\n", input_buf.size);

    if (compress_rle(input_buf.data, input_buf.size, &compressed_buf) != 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    printf("Compressed size: %zu bytes\n", compressed_b