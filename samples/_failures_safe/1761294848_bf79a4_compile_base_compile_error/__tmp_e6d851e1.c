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
    if (buf->data == NULL || data == NULL || len == 0) {
        return 0;
    }
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

int compress_run_length(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) {
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
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!buffer_init(&input_data, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_data);
        return 1;
    }
    if (!buffer_init(&decompressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    uint8_t chunk[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            break;
        }
        if (!buffer_append(&input_data, chunk, bytes_read)) {
            fprintf(stderr, "Input too large or memory error\n");
            buffer_free(&input_data);
            buffer_free(&compressed);
            buffer_free(&decompressed);
            return 1;
        }
        total_read += bytes_read;
    }
    
    if (input_data.size == 0) {
        printf("No input data provided\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_data.size);
    
    if (!compress_run_length(input_data.data, input_data.size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);