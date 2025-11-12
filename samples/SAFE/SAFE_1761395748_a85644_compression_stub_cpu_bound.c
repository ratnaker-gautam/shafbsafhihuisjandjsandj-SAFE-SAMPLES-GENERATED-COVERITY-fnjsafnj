//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
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
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    int c;
    while ((c = getchar()) != EOF && total_read < MAX_INPUT_SIZE) {
        input_buffer[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    if (total_read == MAX_INPUT_SIZE && c != EOF) {
        printf("Input too large. Truncating to %d bytes.\n", MAX_INPUT_SIZE);
    }
    
    size_t total_compressed = 0;
    size_t processed = 0;
    
    while (processed < total_read) {
        size_t chunk_size = (total_read - processed > CHUNK_SIZE) ? CHUNK_SIZE : (total_read - processed);
        
        if (chunk_size == 0) {
            break;
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
    
    double ratio = (double)total_compressed / total_read;
    printf("Original size: %zu bytes\n", total_read);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f\n", ratio);
    
    printf("Compressed data (first 100 bytes): ");
    for (size_t i = 0; i < total_compressed && i < 100; i++) {
        printf("%02X ", output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}