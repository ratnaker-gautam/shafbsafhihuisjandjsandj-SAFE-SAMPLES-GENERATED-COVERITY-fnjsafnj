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

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf->data == NULL || src == NULL) {
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
    uint8_t *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += len;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    while (in_ptr < in_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (out_ptr + 2 > out_end) {
            return 0;
        }
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (uint8_t)count;
        out_ptr++;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t output[MAX_OUTPUT_SIZE];
    Buffer compressed;
    buffer_init(&compressed, 256);
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        buffer_free(&compressed);
        return 1;
    }
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        buffer_free(&compressed);
        return 1;
    }
    for (size_t i = 0; i < input_len; i++) {
        input[i] = (uint8_t)line[i];
    }
    size_t compressed_size = compress_rle(input, input_len, output, sizeof(output));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    if (!buffer_append(&compressed, output, compressed_size)) {
        printf("Buffer append failed\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    buffer_free(&compressed);
    return 0;
}