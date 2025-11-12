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
    memcpy(buf->data + buf->size, src, len);
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
        if (count >= 3) {
            unsigned char marker = 0xFF;
            if (!buffer_append(output, &marker, 1)) return 0;
            if (!buffer_append(output, &current, 1)) return 0;
            unsigned char count_byte = (unsigned char)count;
            if (!buffer_append(output, &count_byte, 1)) return 0;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                unsigned char single = *(input + i + j);
                if (single == 0xFF) {
                    unsigned char escape = 0xFE;
                    if (!buffer_append(output, &escape, 1)) return 0;
                }
                if (!buffer_append(output, &single, 1)) return 0;
            }
            i += count;
        }
    }
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        if (current == 0xFF) {
            if (i + 2 >= input_len) return 0;
            unsigned char value = *(input + i + 1);
            unsigned char count = *(input + i + 2);
            if (count == 0) return 0;
            for (unsigned char j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else if (current == 0xFE) {
            if (i + 1 >= input_len) return 0;
            unsigned char value = *(input + i + 1);
            if (value != 0xFF) return 0;
            if (!buffer_append(output, &value, 1)) return 0;
            i += 2;
        } else {
            if (!buffer_append(output, &current, 1)) return 0;
            i += 1;
        }
    }
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_data[input_len++] = (unsigned char)c;
    }
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_len * 2)) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    if (!compress_rle(input_data, input_len, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed