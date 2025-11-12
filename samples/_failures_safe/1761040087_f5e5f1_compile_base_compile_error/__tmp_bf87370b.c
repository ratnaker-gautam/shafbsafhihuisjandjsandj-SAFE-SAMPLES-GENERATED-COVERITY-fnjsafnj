//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (data_size > MAX_OUTPUT_SIZE - buf->size) {
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
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

int compress_run_length(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
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

int decompress_run_length(const uint8_t *input, size_t input_size, Buffer *output) {
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
    printf("Run-Length Compression/Decompression\n");
    printf("Enter 'c' to compress or 'd' to decompress: ");
    char mode;
    if (scanf(" %c", &mode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (mode != 'c' && mode != 'd') {
        printf("Invalid mode\n");
        return 1;
    }
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    Buffer input_buf;
    if (!buffer_init(&input_buf, 1024)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (input_buf.size >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            buffer_free(&input_buf);
            return 1;
        }
        if (!buffer_append(&input_buf, (uint8_t*)&c, 1)) {
            printf("Memory allocation failed\n");
            buffer_free(&input_buf);
            return 1;
        }
    }
    if (input_buf.size == 0) {
        printf("No input data\n");
        buffer_free(&input_buf);
        return 1;
    }
    Buffer output_buf;
    if (!buffer_init(&output_buf, 1024)) {
        printf("Memory allocation failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    int success = 0;
    if (mode == 'c') {
        success = compress_run_length(input_buf.data, input_buf.size, &output_buf);
        if (success) {
            printf("Compressed data (%zu bytes): ", output_buf.size);
            for (size_t i = 0; i < output_buf.size; i++) {
                printf("%02X", output_buf.data[i]);
            }
            printf("\n");
        }
    } else {
        success = decompress_run_length(input_buf.data