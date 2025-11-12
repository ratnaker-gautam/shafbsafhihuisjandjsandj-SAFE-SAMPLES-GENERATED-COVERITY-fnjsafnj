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
        
        size_t n = fread(temp + total, 1, to_read, stdin);
        if (n == 0) break;
        total += n;
    }
    
    if (total > 0) {
        buf.data = malloc(total);
        if (buf.data) {
            memcpy(buf.data, temp, total);
            buf.size = total;
        }
    }
    
    free(temp);
    return buf;
}

void simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input_size == 0 || !input || !output || !output_size) return;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < input_size - 1) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
            i += count;
        } else {
            for (size_t j = 0; j < count && out_idx < input_size - 1; j++) {
                output[out_idx++] = input[i++];
            }
        }
    }
    
    *output_size = out_idx;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data || input.size == 0) {
        fprintf(stderr, "No input data\n");
        return 1;
    }
    
    uint8_t* compressed = malloc(input.size);
    if (!compressed) {
        free(input.data);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t compressed_size = 0;
    simple_compress(input.data, input.size, compressed, &compressed_size);
    
    if (compressed_size > 0) {
        fwrite(compressed, 1, compressed_size, stdout);
    }
    
    free(input.data);
    free(compressed);
    return 0;
}