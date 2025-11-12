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
    if (input_size == 0 || input_size > CHUNK_SIZE) return 0;
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 2 || (count == 1 && current > 0x7F)) {
            if (output_size + 2 > CHUNK_SIZE) break;
            output[output_size++] = 0xFF;
            output[output_size++] = (uint8_t)count;
            output[output_size++] = current;
            i += count;
        } else {
            if (output_size + 1 > CHUNK_SIZE) break;
            output[output_size++] = current;
            i++;
        }
    }
    
    return output_size;
}

size_t process_input(const uint8_t* input, size_t input_size, Chunk* chunks, size_t max_chunks) {
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    
    size_t total_chunks = 0;
    size_t processed = 0;
    
    while (processed < input_size && total_chunks < max_chunks) {
        size_t chunk_size = (input_size - processed < CHUNK_SIZE) ? 
                           input_size - processed : CHUNK_SIZE;
        
        size_t compressed_size = compress_chunk(input + processed, chunk_size, 
                                              chunks[total_chunks].data);
        
        if (compressed_size == 0) break;
        
        chunks[total_chunks].size = compressed_size;
        processed += chunk_size;
        total_chunks++;
    }
    
    return total_chunks;
}

void generate_test_data(uint8_t* buffer, size_t size) {
    if (size == 0 || size > MAX_INPUT_SIZE) return;
    
    for (size_t i = 0; i < size; i++) {
        buffer[i] = (uint8_t)((i * 7 + 13) % 256);
        if (i > 0 && i % 100 == 0) {
            buffer[i] = buffer[i-1];
        }
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Chunk compressed_chunks[256];
    
    size_t test_size = 8192;
    if (test_size > MAX_INPUT_SIZE) test_size = MAX_INPUT_SIZE;
    
    generate_test_data(input_data, test_size);
    
    printf("Input size: %zu bytes\n", test_size);
    
    size_t num_chunks = process_input(input_data, test_size, compressed_chunks, 256);
    
    if (num_chunks == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < num_chunks; i++) {
        total_compressed += compressed_chunks[i].size;
    }
    
    printf("Compressed into %zu chunks\n", num_chunks);
    printf("Total compressed size: %zu bytes\n", total_compressed);
    
    double ratio = (double)total_compressed / test_size * 100.0;
    printf("Compression ratio: %.2f%%\n", ratio);
    
    if (total_compressed > 0 && total_compressed < test_size) {
        printf("Compression successful\n");
    } else {
        printf("No compression achieved\n");
    }
    
    return 0;
}