//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        int byte = getchar();
        if (byte == EOF) break;
        if (byte < 0 || byte > UCHAR_MAX) continue;
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
    
    if (compressed_size > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Error: Compression overflow\n");
        return 1;
    }
    
    for (size_t i = 0; i < compressed_size; i++) {
        if (putchar(output_buffer[i]) == EOF) {
            fprintf(stderr, "Error: Output write failed\n");
            return 1;
        }
    }
    
    double ratio = (double)compressed_size / total_read * 100.0;
    fprintf(stderr, "Compressed %zu bytes to %zu bytes (%.1f%%)\n", 
            total_read, compressed_size, ratio);
    
    return 0;
}