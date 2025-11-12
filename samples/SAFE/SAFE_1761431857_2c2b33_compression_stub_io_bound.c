//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    unsigned char data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t read_input(unsigned char *buffer, size_t max_size) {
    size_t total_read = 0;
    size_t bytes_read;
    
    while (total_read < max_size) {
        bytes_read = fread(buffer + total_read, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            break;
        }
        total_read += bytes_read;
    }
    
    return total_read;
}

int compress_chunk(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size) {
    if (input_size == 0 || input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < input_size - 1) {
        unsigned char current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (unsigned char)count;
            in_idx += count;
        } else {
            for (size_t i = 0; i < count && out_idx < input_size - 1; i++) {
                output[out_idx++] = input[in_idx++];
            }
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main(void) {
    unsigned char input_buffer[MAX_INPUT_SIZE];
    unsigned char output_buffer[MAX_INPUT_SIZE];
    
    size_t input_size = read_input(input_buffer, MAX_INPUT_SIZE);
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data received\n");
        return 1;
    }
    
    if (input_size >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Error: Input too large\n");
        return 1;
    }
    
    size_t compressed_size;
    int result = compress_chunk(input_buffer, input_size, output_buffer, &compressed_size);
    
    if (result != 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    if (compressed_size >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Error: Output buffer overflow\n");
        return 1;
    }
    
    size_t written = fwrite(output_buffer, 1, compressed_size, stdout);
    if (written != compressed_size) {
        fprintf(stderr, "Error: Failed to write output\n");
        return 1;
    }
    
    double ratio = (double)compressed_size / input_size * 100.0;
    fprintf(stderr, "Compression complete: %zu -> %zu bytes (%.1f%%)\n", input_size, compressed_size, ratio);
    
    return 0;
}