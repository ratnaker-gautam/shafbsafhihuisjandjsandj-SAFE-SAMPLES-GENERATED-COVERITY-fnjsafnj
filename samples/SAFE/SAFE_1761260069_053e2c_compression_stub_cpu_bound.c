//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(Buffer *buf, size_t needed) {
    if (needed > SIZE_MAX / 2) return 0;
    size_t new_capacity = buf->capacity;
    if (new_capacity == 0) new_capacity = 64;
    while (new_capacity < needed) {
        if (new_capacity > SIZE_MAX / 2) return 0;
        new_capacity *= 2;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len > SIZE_MAX - buf->size) return 0;
    if (!buffer_reserve(buf, buf->size + len)) return 0;
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output) {
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (output_pos + 2 > input_len) return 0;
            output[output_pos++] = 0xFF;
            output[output_pos++] = (uint8_t)count;
            output[output_pos++] = current;
            i += count;
        } else {
            if (output_pos + 1 > input_len) return 0;
            output[output_pos++] = current;
            i++;
        }
    }
    
    return output_pos;
}

int process_compression(Buffer *input, Buffer *output) {
    if (input->size == 0) return 0;
    
    if (!buffer_reserve(output, input->size * 2)) return 0;
    
    size_t compressed_size = simple_compress(input->data, input->size, output->data);
    if (compressed_size == 0 || compressed_size > output->capacity) return 0;
    
    output->size = compressed_size;
    return 1;
}

int read_input(Buffer *buf) {
    uint8_t chunk[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            return 0;
        }
        
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) return 0;
        
        if (!buffer_append(buf, chunk, bytes_read)) return 0;
    }
    
    return 1;
}

void write_output(const Buffer *buf) {
    size_t written = 0;
    while (written < buf->size) {
        size_t to_write = buf->size - written;
        if (to_write > CHUNK_SIZE) to_write = CHUNK_SIZE;
        
        size_t result = fwrite(buf->data + written, 1, to_write, stdout);
        if (result != to_write) {
            exit(EXIT_FAILURE);
        }
        written += result;
    }
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        buffer_free(&input);
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    if (!process_compression(&input, &output)) {
        buffer_free(&input);
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    write_output(&output);
    
    buffer_free(&input);
    buffer_free(&output);
    
    return EXIT_SUCCESS;
}