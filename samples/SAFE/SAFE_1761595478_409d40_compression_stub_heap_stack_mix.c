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
    if (input == NULL || output == NULL || out_len == NULL || len == 0) {
        *out_len = 0;
        return;
    }
    size_t j = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && input[i + count] == current && count < UINT8_MAX) {
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
        if (j >= CHUNK_SIZE - 3) {
            break;
        }
    }
    *out_len = j;
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

    uint8_t output_stack[CHUNK_SIZE * 2];
    size_t total_written = 0;

    for (size_t i = 0; i < input_buf.size; i += CHUNK_SIZE) {
        size_t chunk_len = input_buf.size - i;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        size_t compressed_len;
        compress_chunk(input_buf.data + i, chunk_len, output_stack, &compressed_len);
        if (compressed_len > 0) {
            size_t written = fwrite(output_stack, 1, compressed_len, stdout);
            if (written != compressed_len) {
                fprintf(stderr, "Error writing compressed data\n");
                buffer_free(&input_buf);
                return 1;
            }
            total_written += written;
        }
    }

    buffer_free(&input_buf);
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error flushing output\n");
        return 1;
    }
    return 0;
}