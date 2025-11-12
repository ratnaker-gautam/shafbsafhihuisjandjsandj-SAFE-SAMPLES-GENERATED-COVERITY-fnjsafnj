//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input_size == 0 || input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < UCHAR_MAX && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 > MAX_INPUT_SIZE) break;
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            if (out_idx + count > MAX_INPUT_SIZE) break;
            for (size_t i = 0; i < count; i++) {
                output[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    return out_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        int byte = getchar();
        if (byte == EOF) break;
        if (byte < 0 || byte > 255) continue;
        
        input_buffer[total_read++] = (uint8_t)byte;
    }
    
    if (total_read == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    if (total_read == MAX_INPUT_SIZE) {
        fprintf(stderr, "Warning: Input truncated to %d bytes\n", MAX_INPUT_SIZE);
    }
    
    size_t compressed_size = compress_chunk(input_buffer, total_read, output_buffer);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    for (size_t i = 0; i < compressed_size; i++) {
        if (putchar(output_buffer[i]) == EOF) {
            fprintf(stderr, "Error: Output write failed\n");
            return 1;
        }
    }
    
    double ratio = (double)compressed_size / (double)total_read;
    fprintf(stderr, "Compression complete: %zu -> %zu bytes (ratio: %.2f)\n", 
            total_read, compressed_size, ratio);
    
    return 0;
}