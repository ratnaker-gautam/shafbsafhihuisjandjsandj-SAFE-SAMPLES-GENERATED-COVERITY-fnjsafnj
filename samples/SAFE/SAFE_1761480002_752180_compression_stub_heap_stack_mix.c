//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
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
    if (data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
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
    buf->size = new_size;
    return 1;
}

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size == 0 || output == NULL) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        if (count > 3 || current == 0xFF) {
            output[output_index++] = 0xFF;
            output[output_index++] = current;
            output[output_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_index++] = current;
            }
        }
        i += count;
    }
    return output_index;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize buffer\n");
        return 1;
    }
    size_t total_read = 0;
    while (1) {
        size_t bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) {
                break;
            }
            fprintf(stderr, "Read error\n");
            buffer_free(&compressed);
            return 1;
        }
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&compressed);
            return 1;
        }
        uint8_t chunk_output[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(stack_input, bytes_read, chunk_output);
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed\n");
            buffer_free(&compressed);
            return 1;
        }
        if (!buffer_append(&compressed, chunk_output, compressed_size)) {
            fprintf(stderr, "Buffer append failed\n");
            buffer_free(&compressed);
            return 1;
        }
    }
    if (fwrite(compressed.data, 1, compressed.size, stdout) != compressed.size) {
        fprintf(stderr, "Write error\n");
        buffer_free(&compressed);
        return 1;
    }
    buffer_free(&compressed);
    return 0;
}