//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
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
    if (buf == NULL || data == NULL || data_size == 0) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    for (size_t i = 0; i < data_size; i++) {
        *dest++ = *src++;
    }
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    while (current < end) {
        const uint8_t *run_start = current;
        uint8_t value = *current;
        while (current < end && *current == value && (current - run_start) < UINT8_MAX) {
            current++;
        }
        size_t run_length = current - run_start;
        if (run_length > 3 || value == 0xFF) {
            uint8_t control_byte = 0xFF;
            uint8_t length_byte = (uint8_t)run_length;
            if (!buffer_append(output, &control_byte, 1) || !buffer_append(output, &value, 1) || !buffer_append(output, &length_byte, 1)) {
                return 0;
            }
        } else {
            for (size_t i = 0; i < run_length; i++) {
                if (!buffer_append(output, &value, 1)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    printf("Enter input data size (max %d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) {
            printf("Invalid byte input\n");
            return 1;
        }
        if (byte > UINT8_MAX) {
            printf("Byte value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)byte;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, 256)) {
        printf("Failed to initialize output buffer\n");
        return 1;
    }
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_size) * 100.0);
    printf("Compressed data: ");
    uint8_t *ptr = compressed.data;
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *ptr++);
    }
    printf("\n");
    buffer_free(&compressed);
    return 0;
}