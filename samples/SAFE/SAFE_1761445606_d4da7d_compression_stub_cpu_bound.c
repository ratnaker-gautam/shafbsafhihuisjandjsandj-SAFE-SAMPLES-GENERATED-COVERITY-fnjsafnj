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
        return 1;
    }
    
    size_t total_processed = 0;
    size_t total_written = 0;
    
    while (total_processed < total_read) {
        size_t remaining = total_read - total_processed;
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        size_t compressed_size = compress_chunk(
            &input_buffer[total_processed],
            chunk_size,
            &output_buffer[total_written]
        );
        
        if (compressed_size == 0 || total_written + compressed_size > sizeof(output_buffer)) {
            return 1;
        }
        
        total_processed += chunk_size;
        total_written += compressed_size;
    }
    
    for (size_t i = 0; i < total_written; i++) {
        if (putchar(output_buffer[i]) == EOF) {
            return 1;
        }
    }
    
    return 0;
}