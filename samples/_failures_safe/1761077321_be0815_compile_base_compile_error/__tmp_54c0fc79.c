//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

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

int buffer_reserve(Buffer *buf, size_t capacity) {
    if (capacity > MAX_OUTPUT_SIZE) return 0;
    if (buf->capacity >= capacity) return 1;
    
    uint8_t *new_data = realloc(buf->data, capacity);
    if (!new_data) return 0;
    
    buf->data = new_data;
    buf->capacity = capacity;
    return 1;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t size) {
    if (size > MAX_OUTPUT_SIZE - buf->size) return 0;
    if (!buffer_reserve(buf, buf->size + size)) return 0;
    
    memcpy(buf->data + buf->size, data, size);
    buf->size += size;
    return 1;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    
    buffer_init(output);
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) {
            buffer_free(output);
            return 0;
        }
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    if (input_size % 2 != 0) return 0;
    
    buffer_init(output);
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (count == 0) return 0;
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            buffer_free(output);
            return 0;
        }
        
        if (!buffer_reserve(output, output->size + count)) {
            buffer_free(output);
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
    }
    
    return 1;
}

int read_input(Buffer *buf) {
    buffer_init(buf);
    
    if (!buffer_reserve(buf, 4096)) return 0;
    
    size_t total_read = 0;
    while (1) {
        if (total_read >= MAX_INPUT_SIZE) {
            buffer_free(buf);
            return 0;
        }
        
        size_t to_read = buf->capacity - total_read;
        if (to_read == 0) {
            if (!buffer_reserve(buf, buf->capacity * 2)) {
                buffer_free(buf);
                return 0;
            }
            to_read = buf->capacity - total_read;
        }
        
        size_t bytes_read = fread(buf->data + total_read, 1, to_read, stdin);
        total_read += bytes_read;
        buf->size = total_read;
        
        if (bytes_read < to_read) {
            if (ferror(stdin)) {
                buffer_free(buf);
                return 0;
            }
            break;
        }
    }
    
    return 1;
}

void write_output(const Buffer *buf) {
    if (!buf || !buf->data) return;
    fwrite(buf->data, 1, buf->size, stdout);
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    printf("Enter data to compress (end with Ctrl+D): ");
    fflush(stdout);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        return 1;
    }
    
    if (!compress_rle(input.data, input.size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    write_output(&compressed);
    printf("\n");
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return 1;
    }