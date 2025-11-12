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
    
    printf("Enter data to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    int c;
    while (total_input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        if (c < 0 || c > 255) {
            continue;
        }
        input_buffer[total_input_size++] = (uint8_t)c;
        
        if (total_input_size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    
    if (total_input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Processing %zu bytes...\n", total_input_size);
    
    size_t total_compressed = 0;
    size_t processed = 0;
    
    while (processed < total_input_size) {
        size_t chunk_size = total_input_size - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + processed, chunk_size, compressed_chunk);
        
        if (compressed_size > 0 && total_compressed + compressed_size < sizeof(output_buffer)) {
            memcpy(output_buffer + total_compressed, compressed_chunk, compressed_size);
            total_compressed += compressed_size;
        } else {
            printf("Compression buffer overflow.\n");
            return 1;
        }
        
        processed += chunk_size;
    }
    
    double ratio = (double)total_compressed / total_input_size;
    printf("Compression complete.\n");
    printf("Original size: %zu bytes\n", total_input_size);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f\n", ratio);
    
    printf("Compressed data (first 100 bytes): ");
    for (size_t i = 0; i < total_compressed && i < 100; i++) {
        printf("%02X ", output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}