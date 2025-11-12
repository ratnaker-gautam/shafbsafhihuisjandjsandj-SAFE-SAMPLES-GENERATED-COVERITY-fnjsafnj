//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size % 2 != 0) {
        return -1;
    }
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
    
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / input_size * 100.0f);
    
    size_t decompressed_size;
    if (decompress_rle(&compressed, output_data, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input_data, output_data, input_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompression successful\n");
    printf("Original: %s\n", input_data);
    printf("Decompressed: %s\n", output_data);
    
    return 0;
}