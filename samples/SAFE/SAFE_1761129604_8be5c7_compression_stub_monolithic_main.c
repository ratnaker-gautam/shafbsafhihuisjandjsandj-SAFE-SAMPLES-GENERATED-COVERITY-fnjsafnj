//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               input[i + count] == current && output->size < MAX_OUTPUT_SIZE - 2) {
            count++;
        }
        
        if (count >= 3) {
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && output->size < MAX_OUTPUT_SIZE; j++) {
                output->data[output->size++] = current;
            }
            i += count;
        }
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_INPUT_SIZE) {
        if (in_idx + 2 < input->size && input->data[in_idx] == 0xFF) {
            uint8_t value = input->data[in_idx + 1];
            uint8_t count = input->data[in_idx + 2];
            
            if (out_idx + count > MAX_INPUT_SIZE) {
                return -1;
            }
            
            for (uint8_t j = 0; j < count && out_idx < MAX_INPUT_SIZE; j++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output[out_idx++] = input->data[in_idx++];
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)original);
    if (input_size > 0 && original[input_size - 1] == '\n') {
        original[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (compress_rle(original, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) * 100.0f / input_size);
    
    size_t decompressed_size;
    if (decompress_rle(&compressed, decompressed, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_size || 
        memcmp(original, decompressed, input_size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: %s\n", original);
    printf("Decompressed: %s\n", decompressed);
    
    return 0;
}