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
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
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

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    
    printf("Enter data size (max %d): ", MAX_INPUT_SIZE);
    size_t input_size;
    
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int byte;
        if (scanf("%d", &byte) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return 1;
        }
        if (byte < 0 || byte > 255) {
            fprintf(stderr, "Byte value out of range\n");
            return 1;
        }
        input_buffer[i] = (uint8_t)byte;
    }
    
    if (!validate_input(input_buffer, input_size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    size_t processed = 0;
    
    while (processed < input_size) {
        size_t chunk_size = (input_size - processed > CHUNK_SIZE) ? CHUNK_SIZE : (input_size - processed);
        
        if (chunk_size == 0) {
            break;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + processed, chunk_size, compressed_chunk);
        
        if (compressed_size == 0 || compressed_size > sizeof(compressed_chunk)) {
            fprintf(stderr, "Compression error\n");
            return 1;
        }
        
        if (total_compressed + compressed_size > sizeof(output_buffer)) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        memcpy(output_buffer + total_compressed, compressed_chunk, compressed_size);
        total_compressed += compressed_size;
        processed += chunk_size;
    }
    
    double compression_ratio = (double)total_compressed / (double)input_size;
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f\n", compression_ratio);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < total_compressed; i++) {
        printf("%d ", output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}