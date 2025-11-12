//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
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
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (output_size + 2 > CHUNK_SIZE) {
                return 0;
            }
            output[output_size++] = 0xFF;
            output[output_size++] = (uint8_t)count;
            output[output_size++] = current;
        } else {
            if (output_size + count > CHUNK_SIZE) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_size++] = input[i + j];
            }
        }
        
        i += count;
    }
    
    return output_size;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    size_t total_input_size = 0;
    size_t total_output_size = 0;
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && total_input_size < MAX_INPUT_SIZE) {
        input_buffer[total_input_size++] = (uint8_t)c;
    }
    
    if (total_input_size == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    if (total_input_size == MAX_INPUT_SIZE && c != EOF) {
        fprintf(stderr, "Error: Input too large\n");
        return 1;
    }
    
    size_t remaining = total_input_size;
    size_t input_pos = 0;
    
    while (remaining > 0) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + input_pos, chunk_size, compressed_chunk);
        
        if (compressed_size == 0) {
            fprintf(stderr, "Error: Compression failed\n");
            return 1;
        }
        
        if (total_output_size + compressed_size > sizeof(output_buffer)) {
            fprintf(stderr, "Error: Output buffer overflow\n");
            return 1;
        }
        
        memcpy(output_buffer + total_output_size, compressed_chunk, compressed_size);
        total_output_size += compressed_size;
        input_pos += chunk_size;
        remaining -= chunk_size;
    }
    
    double ratio = (double)total_output_size / total_input_size;
    
    printf("Original size: %zu bytes\n", total_input_size);
    printf("Compressed size: %zu bytes\n", total_output_size);
    printf("Compression ratio: %.2f\n", ratio);
    
    printf("Compressed data (first 100 bytes): ");
    for (size_t i = 0; i < total_output_size && i < 100; i++) {
        printf("%02X ", output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}