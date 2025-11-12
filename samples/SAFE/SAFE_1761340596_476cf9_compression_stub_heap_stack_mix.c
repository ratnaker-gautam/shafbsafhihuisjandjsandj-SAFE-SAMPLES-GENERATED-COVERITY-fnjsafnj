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

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size == 0 || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (out_idx + 2 > CHUNK_SIZE) {
            break;
        }
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    *output_size = out_idx;
}

int main(void) {
    Buffer input_buf;
    if (!buffer_init(&input_buf, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }

    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input_buf, chunk, bytes_read)) {
            fprintf(stderr, "Input too large or memory error\n");
            buffer_free(&input_buf);
            return 1;
        }
    }
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input_buf);
        return 1;
    }

    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_buf);
        return 1;
    }

    uint8_t compressed_stack[CHUNK_SIZE * 2];
    size_t compressed_size;
    compress_chunk(input_buf.data, input_buf.size, compressed_stack, &compressed_size);

    if (compressed_size > 0) {
        size_t written = fwrite(compressed_stack, 1, compressed_size, stdout);
        if (written != compressed_size) {
            fprintf(stderr, "Error writing output\n");
            buffer_free(&input_buf);
            return 1;
        }
    } else {
        fprintf(stderr, "Compression produced no output\n");
        buffer_free(&input_buf);
        return 1;
    }

    buffer_free(&input_buf);
    return 0;
}