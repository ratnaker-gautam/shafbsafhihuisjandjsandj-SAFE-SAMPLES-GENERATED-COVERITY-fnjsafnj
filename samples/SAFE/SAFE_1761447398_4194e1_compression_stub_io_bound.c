//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
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

size_t compress_data(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        compressed_size += 2;
        i += count;
    }
    return compressed_size;
}

int read_input_data(uint8_t *buffer, size_t max_size, size_t *actual_size) {
    if (buffer == NULL || actual_size == NULL) {
        return 0;
    }
    size_t total_read = 0;
    while (total_read < max_size) {
        int ch = getchar();
        if (ch == EOF) {
            break;
        }
        buffer[total_read] = (uint8_t)ch;
        total_read++;
    }
    *actual_size = total_read;
    return 1;
}

int write_output_data(const uint8_t *data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    for (size_t i = 0; i < size; i++) {
        if (putchar(data[i]) == EOF) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    if (!read_input_data(input_buffer, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input data to compress\n");
        return 1;
    }
    
    Buffer output_buffer;
    if (!buffer_init(&output_buffer, 1024)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t compressed_size = compress_data(input_buffer, input_size, &output_buffer);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&output_buffer);
        return 1;
    }
    
    if (!write_output_data(output_buffer.data, output_buffer.size)) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&output_buffer);
        return 1;
    }
    
    buffer_free(&output_buffer);
    return 0;
}