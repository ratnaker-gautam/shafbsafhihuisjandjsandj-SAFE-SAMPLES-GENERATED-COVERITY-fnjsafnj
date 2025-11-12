//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer* buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(Buffer* buf, size_t needed) {
    if (needed > SIZE_MAX / 2) return 0;
    size_t new_capacity = buf->capacity;
    while (new_capacity < needed) {
        new_capacity = new_capacity ? new_capacity * 2 : CHUNK_SIZE;
        if (new_capacity > MAX_INPUT_SIZE) return 0;
    }
    uint8_t* new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t len) {
    if (len > SIZE_MAX - buf->size) return 0;
    if (!buffer_reserve(buf, buf->size + len)) return 0;
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void buffer_free(Buffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (output_pos + 2 > input_size) return 0;
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
        } else {
            if (output_pos + count > input_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[output_pos++] = current;
            }
        }
        
        i += count;
    }
    
    return output_pos;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    buffer_init(&input);
    buffer_init(&output);
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input, chunk, bytes_read)) {
            fprintf(stderr, "Error: Input too large\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error: Failed to read input\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Error: No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (!buffer_reserve(&output, input.size * 2)) {
        fprintf(stderr, "Error: Output buffer allocation failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, output.data);
    if (compressed_size == 0 || compressed_size > output.capacity) {
        fprintf(stderr, "Error: Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    output.size = compressed_size;
    
    size_t written = fwrite(output.data, 1, output.size, stdout);
    if (written != output.size) {
        fprintf(stderr, "Error: Failed to write output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error: Failed to flush output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    fprintf(stderr, "Compressed %zu bytes to %zu bytes\n", input.size, output.size);
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}