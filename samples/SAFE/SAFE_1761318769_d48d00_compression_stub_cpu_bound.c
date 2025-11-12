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
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
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
        
        if (output_idx >= CHUNK_SIZE - 3) {
            break;
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    size_t total_input_size = 0;
    size_t total_output_size = 0;
    
    printf("Enter data to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    int c;
    while (total_input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        input_buffer[total_input_size++] = (uint8_t)c;
    }
    
    if (total_input_size == 0) {
        fprintf(stderr, "Error: No input data provided\n");
        return 1;
    }
    
    if (total_input_size == MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        fprintf(stderr, "Error: Input exceeds maximum size\n");
        return 1;
    }
    
    size_t remaining = total_input_size;
    size_t input_pos = 0;
    
    while (remaining > 0) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        size_t compressed_size = compress_chunk(&input_buffer[input_pos], chunk_size, 
                                              &output_buffer[total_output_size]);
        
        if (compressed_size == 0 || compressed_size > MAX_INPUT_SIZE * 2 - total_output_size) {
            fprintf(stderr, "Error: Compression failed or output buffer overflow\n");
            return 1;
        }
        
        total_output_size += compressed_size;
        input_pos += chunk_size;
        remaining -= chunk_size;
    }
    
    printf("Original size: %zu bytes\n", total_input_size);
    printf("Compressed size: %zu bytes\n", total_output_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)total_output_size / total_input_size * 100.0);
    
    printf("Compressed data (hex):\n");
    for (size_t i = 0; i < total_output_size && i < 256; i++) {
        printf("%02X ", output_buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (total_output_size > 0 && total_output_size % 16 != 0) {
        printf("\n");
    }
    
    if (total_output_size > 256) {
        printf("... (truncated)\n");
    }
    
    return 0;
}