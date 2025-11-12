//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    uint8_t* temp = malloc(MAX_INPUT_SIZE);
    if (!temp) return buf;
    
    size_t total = 0;
    while (total < MAX_INPUT_SIZE) {
        size_t to_read = MAX_INPUT_SIZE - total;
        if (to_read > CHUNK_SIZE) to_read = CHUNK_SIZE;
        
        size_t bytes_read = fread(temp + total, 1, to_read, stdin);
        if (bytes_read == 0) break;
        total += bytes_read;
    }
    
    if (total > 0) {
        buf.data = temp;
        buf.size = total;
    } else {
        free(temp);
    }
    
    return buf;
}

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!input || !output || !output_size || input_size == 0) return -1;
    if (input_size > MAX_INPUT_SIZE) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < MAX_INPUT_SIZE - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (out_idx + 2 >= MAX_INPUT_SIZE) break;
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            if (out_idx + 1 >= MAX_INPUT_SIZE) break;
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data || input.size == 0) {
        fprintf(stderr, "Error: No input data or read failure\n");
        return 1;
    }
    
    uint8_t* compressed = malloc(MAX_INPUT_SIZE);
    if (!compressed) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(input.data);
        return 1;
    }
    
    size_t compressed_size = 0;
    int result = compress_data(input.data, input.size, compressed, &compressed_size);
    
    if (result != 0) {
        fprintf(stderr, "Error: Compression failed\n");
        free(input.data);
        free(compressed);
        return 1;
    }
    
    size_t written = fwrite(compressed, 1, compressed_size, stdout);
    if (written != compressed_size) {
        fprintf(stderr, "Error: Output write failed\n");
        free(input.data);
        free(compressed);
        return 1;
    }
    
    free(input.data);
    free(compressed);
    return 0;
}