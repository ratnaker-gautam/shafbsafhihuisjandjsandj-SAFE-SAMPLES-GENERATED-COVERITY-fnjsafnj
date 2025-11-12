//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
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
        
        if (count >= 3) {
            if (output_idx + 2 > CHUNK_SIZE) {
                break;
            }
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
            i += count;
        } else {
            if (output_idx + count > CHUNK_SIZE) {
                break;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = input[i + j];
            }
            i += count;
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        int byte = getchar();
        if (byte == EOF) {
            break;
        }
        if (byte < 0 || byte > 255) {
            continue;
        }
        input_buffer[total_read++] = (uint8_t)byte;
    }
    
    if (total_read == 0) {
        return 0;
    }
    
    size_t total_processed = 0;
    uint8_t output_buffer[CHUNK_SIZE * 2];
    
    while (total_processed < total_read) {
        size_t remaining = total_read - total_processed;
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + total_processed, chunk_size, compressed);
        
        if (compressed_size > 0) {
            for (size_t i = 0; i < compressed_size; i++) {
                if (putchar(compressed[i]) == EOF) {
                    return 1;
                }
            }
        }
        
        total_processed += chunk_size;
    }
    
    return 0;
}