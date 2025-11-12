//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf->data == NULL || data == NULL || len == 0) {
        return 0;
    }
    if (len > MAX_OUTPUT_SIZE - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
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
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        i += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        if (count == 0) {
            return 0;
        }
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    printf("Simple RLE Compression/Decompression\n");
    printf("Enter 'c' to compress or 'd' to decompress: ");
    char mode;
    if (scanf(" %c", &mode) != 1) {
        printf("Error reading mode\n");
        return 1;
    }
    if (mode != 'c' && mode != 'd') {
        printf("Invalid mode\n");
        return 1;
    }
    printf("Enter input data (max %d bytes as hex digits, no spaces): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (scanf("%s", hex_input) != 1) {
        printf("Error reading input\n");
        return 1;
    }
    size_t hex_len = strlen(hex_input);
    if (hex_len % 2 != 0) {
        printf("Invalid hex input length\n");
        return 1;
    }
    size_t input_size = hex_len / 2;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of bounds\n");
        return 1;
    }
    uint8_t *input_data = malloc(input_size);
    if (input_data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    for (size_t i = 0; i < input_size; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        unsigned int byte_val;
        if (sscanf(hex_byte, "%02x", &byte_val) != 1 || byte_val > 255) {
            printf("Invalid hex byte: %s\n", hex_byte);
            free(input_data);
            return 1;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    Buffer output;
    if (!buffer