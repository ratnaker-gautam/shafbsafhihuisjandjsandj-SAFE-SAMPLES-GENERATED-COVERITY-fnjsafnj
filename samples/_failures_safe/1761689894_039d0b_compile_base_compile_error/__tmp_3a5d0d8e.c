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
    if (initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!data || len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > MAX_INPUT_SIZE) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    if (!buffer_init(output, input_size * 2)) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        if (count >= 3 || current == 0xFF) {
            uint8_t seq[3] = {0xFF, current, (uint8_t)count};
            if (!buffer_append(output, seq, 3)) {
                buffer_free(output);
                return 0;
            }
        } else {
            for (size_t j = 0; j < count; j++) {
                if (!buffer_append(output, &input[i + j], 1)) {
                    buffer_free(output);
                    return 0;
                }
            }
        }
        i += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    if (!buffer_init(output, input_size * 2)) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) {
                    buffer_free(output);
                    return 0;
                }
            }
            i += 3;
        } else {
            if (!buffer_append(output, &input[i], 1)) {
                buffer_free(output);
                return 0;
            }
            i++;
        }
    }
    return 1;
}

int read_input(Buffer *buf) {
    if (!buffer_init(buf, CHUNK_SIZE)) return 0;
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) {
            buffer_free(buf);
            return 0;
        }
        if (buf->size >= MAX_INPUT_SIZE) break;
    }
    if (ferror(stdin)) {
        buffer_free(buf);
        return 0;
    }
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    printf("Enter data to compress (end with Ctrl+D):\n");
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        return 1;
    }
    
    printf("Compressing %zu bytes...\n", input.size);
    if (!compress_rle(input.data, input.size, &output)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        return 1;
    }
    
    printf("Compressed to %zu bytes (%.1f%%)\n", output.size, 
           (double)output.size / input.size * 100);
    
    Buffer decompressed = {0};
    if (!decompress_rle(output.data, output.size