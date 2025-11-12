//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

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
    if (buf->data == NULL || data == NULL || len == 0) {
        return 0;
    }
    if (len > SIZE_MAX - buf->size) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            return 0;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            new_capacity = MAX_INPUT_SIZE;
        }
        if (new_size > new_capacity) {
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

size_t compress_chunk(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    size_t output_pos = 0;
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        if (count >= 3 || current == 0xFF) {
            if (output_pos + 3 > CHUNK_SIZE) {
                break;
            }
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
            i += count;
        } else {
            if (output_pos + 1 > CHUNK_SIZE) {
                break;
            }
            output[output_pos++] = current;
            i++;
        }
    }
    return output_pos;
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
            fprintf(stderr, "Failed to append data to buffer\n");
            buffer_free(&input_buf);
            return 1;
        }
    }
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading from stdin\n");
        buffer_free(&input_buf);
        return 1;
    }

    if (input_buf.size == 0) {
        fprintf(stderr, "No input data provided\n");
        buffer_free(&input_buf);
        return 1;
    }

    Buffer output_buf;
    if (!buffer_init(&output_buf, input_buf.size)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }

    size_t total_processed = 0;
    while (total_processed < input_buf.size) {
        size_t remaining = input_buf.size - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        uint8_t compressed[CHUNK_SIZE];
        size_t compressed_size = compress_chunk(input_buf.data + total_processed, chunk_size, compressed);
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed\n");
            buffer_free(&input_buf);
            buffer_free(&output_buf);
            return 1;
        }
        if (!buffer_append(&output_buf, compressed, compressed_size)) {
            fprintf(stderr, "Failed to append compressed data\n");
            buffer_free(&input_buf);
            buffer_free(&output_buf);
            return 1;
        }
        total_processed += chunk_size;
    }

    size_t written = fwrite(output_buf.data, 1, output_buf.size, stdout);
    if (written != output_buf.size)