//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
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
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
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
    size_t total_read = 0;
    size_t total_compressed = 0;
    
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
        return 1;
    }
    
    size_t remaining = total_read;
    size_t input_pos = 0;
    
    while (remaining > 0) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        size_t compressed_size = compress_chunk(input_buffer + input_pos, chunk_size, 
                                              output_buffer + total_compressed);
        
        if (compressed_size == 0 || compressed_size > MAX_INPUT_SIZE * 2 - total_compressed) {
            return 1;
        }
        
        total_compressed += compressed_size;
        input_pos += chunk_size;
        remaining -= chunk_size;
    }
    
    for (size_t i = 0; i < total_compressed; i++) {
        if (putchar(output_buffer[i]) == EOF) {
            return 1;
        }
    }
    
    return 0;
}