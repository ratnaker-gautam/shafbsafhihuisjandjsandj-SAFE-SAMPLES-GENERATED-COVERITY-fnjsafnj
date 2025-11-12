//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
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
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
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

int compress_run_length(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && *(input + i + count) == current && count < UCHAR_MAX) {
            count++;
        }
        unsigned char run[2] = {current, (unsigned char)count};
        if (!buffer_append(output, run, 2)) return 0;
        i += count;
    }
    return 1;
}

int decompress_run_length(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return 0;
    size_t i = 0;
    while (i < input_len) {
        unsigned char value = *(input + i);
        unsigned char count = *(input + i + 1);
        if (count == 0) return 0;
        for (unsigned char j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
        i += 2;
    }
    return 1;
}

int main(void) {
    unsigned char input_data[BUFFER_SIZE];
    printf("Enter text to compress (max %d characters): ", BUFFER_SIZE - 1);
    if (fgets((char*)input_data, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Input is empty\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, input_len * 2)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    if (!compress_run_length(input_data, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *(compressed.data + i));
    }
    printf("\n");
    Buffer decompressed;
    if (!buffer_init(&decompressed, input_len * 2)) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    if (!decompress_run_length(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", *(decompressed.data + i));
    }
    printf("\n");
    if (