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
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_size + 3 > CHUNK_SIZE) break;
            output[output_size++] = 0xFF;
            output[output_size++] = (uint8_t)count;
            output[output_size++] = current;
        } else {
            if (output_size + count > CHUNK_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[output_size++] = current;
            }
        }
        
        i += count;
    }
    
    return output_size;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_INPUT_SIZE * 2];
    
    size_t input_size = 0;
    int c;
    
    while ((c = getchar()) != EOF && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data provided\n");
        return 1;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Error: Invalid input data\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    size_t processed = 0;
    
    while (processed < input_size) {
        size_t chunk_size = (input_size - processed > CHUNK_SIZE) ? CHUNK_SIZE : (input_size - processed);
        
        if (chunk_size == 0) break;
        
        uint8_t chunk_output[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_data + processed, chunk_size, chunk_output);
        
        if (compressed_size == 0) {
            fprintf(stderr, "Error: Compression failed for chunk\n");
            return 1;
        }
        
        if (total_compressed + compressed_size > sizeof(compressed_data)) {
            fprintf(stderr, "Error: Output buffer overflow\n");
            return 1;
        }
        
        memcpy(compressed_data + total_compressed, chunk_output, compressed_size);
        total_compressed += compressed_size;
        processed += chunk_size;
    }
    
    if (fwrite(compressed_data, 1, total_compressed, stdout) != total_compressed) {
        fprintf(stderr, "Error: Failed to write output\n");
        return 1;
    }
    
    return 0;
}