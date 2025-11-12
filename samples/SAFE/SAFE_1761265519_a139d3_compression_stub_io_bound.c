//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

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
    if (len > MAX_INPUT_SIZE - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
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
    buf->size += len;
    return 1;
}

int compress_data(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t output_pos = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (count >= 3) {
            if (output_pos + 2 > output->capacity) {
                if (!buffer_append(output, (uint8_t[]){0x00}, 1)) return 0;
                output_pos++;
            }
            if (!buffer_append(output, (uint8_t[]){0xFF, current, (uint8_t)count}, 3)) return 0;
            output_pos += 3;
            i += count;
        } else {
            if (output_pos + 1 > output->capacity) {
                if (!buffer_append(output, (uint8_t[]){0x00}, 1)) return 0;
                output_pos++;
            }
            if (!buffer_append(output, &current, 1)) return 0;
            output_pos++;
            i++;
        }
    }
    return 1;
}

int read_input_data(Buffer *input) {
    if (!buffer_init(input, CHUNK_SIZE)) {
        return 0;
    }
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            buffer_free(input);
            return 0;
        }
        if (input->size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    if (ferror(stdin)) {
        buffer_free(input);
        return 0;
    }
    return 1;
}

int main(void) {
    Buffer input_data;
    Buffer compressed_data;
    
    if (!buffer_init(&compressed_data, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!read_input_data(&input_data)) {
        fprintf(stderr, "Error reading input data\n");
        buffer_free(&compressed_data);
        return 1;
    }
    
    if (input_data.size == 0) {
        fprintf(stderr, "No input data provided\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    if (!compress_data(input_data.data, input_data.size, &compressed_data)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    size_t written = fwrite(compressed_data.data, 1, compressed_data.size, stdout);
    if (written != compressed_data.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    buffer_free(&input_data);
    buffer_free(&compressed_data);
    
    return 0;
}