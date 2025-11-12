//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (output_size + 2 > CHUNK_SIZE) break;
            output[output_size++] = 0xFF;
            output[output_size++] = (uint8_t)count;
            output[output_size++] = current;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (output_size >= CHUNK_SIZE) break;
                if (current == 0xFF && output_size + 1 < CHUNK_SIZE) {
                    output[output_size++] = 0xFF;
                    output[output_size++] = 1;
                } else {
                    output[output_size++] = current;
                }
            }
        }
        
        i += count;
    }
    
    return output_size;
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    size_t total_input_size = 0;
    size_t total_output_size = 0;
    
    int c;
    while ((c = getchar()) != EOF && total_input_size < MAX_INPUT_SIZE) {
        input_buffer[total_input_size++] = (uint8_t)c;
    }
    
    if (total_input_size == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    if (total_input_size == MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        fprintf(stderr, "Error: Input too large\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < total_input_size) {
        size_t chunk_size = (total_input_size - processed > CHUNK_SIZE) ? CHUNK_SIZE : (total_input_size - processed);
        
        if (chunk_size == 0) break;
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + processed, chunk_size, compressed_chunk);
        
        if (total_output_size + compressed_size > sizeof(output_buffer)) {
            fprintf(stderr, "Error: Output buffer overflow\n");
            return 1;
        }
        
        memcpy(output_buffer + total_output_size, compressed_chunk, compressed_size);
        total_output_size += compressed_size;
        processed += chunk_size;
    }
    
    size_t written = fwrite(output_buffer, 1, total_output_size, stdout);
    if (written != total_output_size) {
        fprintf(stderr, "Error: Write failure\n");
        return 1;
    }
    
    double ratio = (double)total_output_size / total_input_size;
    fprintf(stderr, "Compressed %zu bytes to %zu bytes (ratio: %.2f)\n", 
            total_input_size, total_output_size, ratio);
    
    return 0;
}