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
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += len;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        if (count > 3 || current == 0xFF) {
            if (output->size + 3 > MAX_OUTPUT_SIZE) return 0;
            unsigned char marker = 0xFF;
            unsigned char count_byte = (unsigned char)count;
            if (!buffer_append(output, &marker, 1)) return 0;
            if (!buffer_append(output, &count_byte, 1)) return 0;
            if (!buffer_append(output, &current, 1)) return 0;
        } else {
            if (output->size + count > MAX_OUTPUT_SIZE) return 0;
            for (size_t j = 0; j < count; j++) {
                if (!buffer_append(output, input + i + j, 1)) return 0;
            }
        }
        i += count;
    }
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        if (current == 0xFF && i + 2 < input_len) {
            unsigned char count = *(input + i + 1);
            unsigned char value = *(input + i + 2);
            if (count == 0) return 0;
            if (output->size + count > MAX_OUTPUT_SIZE) return 0;
            for (size_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else {
            if (output->size + 1 > MAX_OUTPUT_SIZE) return 0;
            if (!buffer_append(output, input + i, 1)) return 0;
            i++;
        }
    }
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input data (up to %d bytes, end with EOF): ", MAX_INPUT_SIZE);
    int ch;
    while ((ch = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input_data[input_len] = (unsigned char)ch;
        input_len++;
    }
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, 256)) {
        printf("Failed to initialize compression buffer.\n");
        return 1;
    }
    
    if (!compress_rle(input_data, input_len, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / (float)input_len * 100.0);