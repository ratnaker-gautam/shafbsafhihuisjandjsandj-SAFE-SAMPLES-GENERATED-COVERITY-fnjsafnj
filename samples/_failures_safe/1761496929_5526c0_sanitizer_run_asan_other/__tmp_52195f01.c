//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 2 > CHUNK_SIZE) return 0;
            output[output_idx++] = 0xFF;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            if (output_idx + count > CHUNK_SIZE) return 0;
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
    size_t total_input_size = 0;
    
    while (1) {
        if (total_input_size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input size limit exceeded\n");
            return 1;
        }
        
        size_t bytes_read = fread(input_buffer + total_input_size, 1, 
                                 CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            break;
        }
        
        total_input_size += bytes_read;
        if (total_input_size > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input size limit exceeded\n");
            return 1;
        }
    }
    
    if (total_input_size == 0) {
        fprintf(stderr, "No input data\n");
        return 1;
    }
    
    size_t total_output_size = 0;
    size_t processed = 0;
    
    while (processed < total_input_size) {
        size_t chunk_size = (total_input_size - processed < CHUNK_SIZE) ? 
                           total_input_size - processed : CHUNK_SIZE;
        
        if (chunk_size == 0) break;
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + processed, 
                                               chunk_size, compressed_chunk);
        
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        
        if (total_output_size + compressed_size > sizeof(output_buffer)) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        memcpy(output_buffer + total_output_size, compressed_chunk, compressed_size);
        total_output_size += compressed_size;
        processed += chunk_size;
    }
    
    size_t written = fwrite(output_buffer, 1, total_output_size, stdout);
    if (written != total_output_size) {
        fprintf(stderr, "Output write failed\n");
        return 1;
    }
    
    double ratio = (double)total_output_size / total_input_size;
    fprintf(stderr, "Compressed %zu bytes to %zu bytes (ratio: %.2f)\n",
            total_input_size, total_output_size, ratio);
    
    return 0;
}