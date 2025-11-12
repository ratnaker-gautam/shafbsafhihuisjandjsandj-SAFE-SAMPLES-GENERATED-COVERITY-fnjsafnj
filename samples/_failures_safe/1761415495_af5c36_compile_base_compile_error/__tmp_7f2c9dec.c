//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) new_capacity = buf->size + data_size;
        if (new_capacity > SIZE_MAX) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size > SIZE_MAX / 2) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = *(input + i);
        size_t count = 1;
        while (i + count < input_size && count < 255 && *(input + i + count) == current) {
            count++;
        }
        if (count >= 3) {
            uint8_t run[2] = {0xFF, current};
            if (!buffer_append(output, run, 2)) return 0;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_size && (i - literal_start < 255) && 
                   (i + 1 >= input_size || *(input + i) != *(input + i + 1) || 
                    i + 2 >= input_size || *(input + i) != *(input + i + 2))) {
                i++;
            }
            size_t literal_length = i - literal_start;
            if (literal_length > 0) {
                if (literal_length > 255) literal_length = 255;
                uint8_t length_byte = (uint8_t)literal_length;
                if (!buffer_append(output, &length_byte, 1)) return 0;
                if (!buffer_append(output, input + literal_start, literal_length)) return 0;
            }
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t control = *(input + i);
        i++;
        if (control == 0xFF) {
            if (i >= input_size) return 0;
            uint8_t value = *(input + i);
            i++;
            uint8_t repeated[255];
            memset(repeated, value, sizeof(repeated));
            if (!buffer_append(output, repeated, 255)) return 0;
        } else {
            size_t literal_length = control;
            if (i + literal_length > input_size) return 0;
            if (!buffer_append(output, input + i, literal_length)) return 0;
            i += literal_length;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_size * 2)) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    if (!compress_rle(input, input_size, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    double ratio = (compressed.size * 100.0)