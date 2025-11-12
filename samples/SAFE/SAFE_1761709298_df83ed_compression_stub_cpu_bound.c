//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
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
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    size_t i = 0;
    size_t j = 0;
    while (i < len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        if (count > 3 || current == 0xFF) {
            output[j++] = 0xFF;
            output[j++] = current;
            output[j++] = (uint8_t)count;
        } else {
            for (size_t k = 0; k < count; k++) {
                output[j++] = current;
            }
        }
        i += count;
    }
    *out_len = j;
}

int main(void) {
    Buffer input = {0};
    if (!buffer_init(&input, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }

    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input, chunk, bytes_read)) {
            fprintf(stderr, "Input too large or memory error\n");
            buffer_free(&input);
            return 1;
        }
    }
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input);
        return 1;
    }

    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        return 1;
    }

    size_t max_output_size = input.size * 2;
    if (max_output_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Output too large\n");
        buffer_free(&input);
        return 1;
    }

    uint8_t *output = malloc(max_output_size);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }

    size_t output_len;
    compress_chunk(input.data, input.size, output, &output_len);

    size_t written = fwrite(output, 1, output_len, stdout);
    if (written != output_len) {
        fprintf(stderr, "Error writing output\n");
        free(output);
        buffer_free(&input);
        return 1;
    }

    free(output);
    buffer_free(&input);
    return 0;
}