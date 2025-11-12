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
        if (new_capacity < new_size) {
            new_capacity = new_size;
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

void compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) {
        return;
    }
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        unsigned char compressed[2] = {value, (unsigned char)count};
        buffer_append(output, compressed, 2);
        current = next;
    }
}

void decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size % 2 != 0) {
        return;
    }
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    while (current < end) {
        unsigned char value = *current;
        current++;
        unsigned char count = *current;
        current++;
        for (unsigned char i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    Buffer compressed;
    buffer_init(&compressed, 64);
    compress_rle((unsigned char*)input_str, input_len, &compressed);
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    Buffer decompressed;
    buffer_init(&decompressed, 64);
    decompress_rle(compressed.data, compressed.size, &decompressed);
    printf("Decompressed text: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf("\n");
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return EXIT_SUCCESS;
}