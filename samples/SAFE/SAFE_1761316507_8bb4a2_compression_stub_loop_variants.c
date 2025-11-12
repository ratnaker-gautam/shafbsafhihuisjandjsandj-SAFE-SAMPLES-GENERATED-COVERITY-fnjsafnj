//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t k = 0; k < count; k++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

size_t simple_decompress(const CompressedData* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx >= output_size) {
            return 0;
        }
        
        if (input->data[in_idx] == 0xFF && in_idx + 2 < input->size) {
            uint8_t value = input->data[in_idx + 1];
            uint8_t count = input->data[in_idx + 2];
            
            if (out_idx + count > output_size) {
                return 0;
            }
            
            for (uint8_t k = 0; k < count; k++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output[out_idx++] = input->data[in_idx++];
        }
    }
    
    return out_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memset(&compressed, 0, sizeof(compressed));
    
    size_t compressed_size = simple_compress(input_buffer, input_len, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_len) * 100.0);
    
    memset(output_buffer, 0, sizeof(output_buffer));
    size_t decompressed_size = simple_decompress(&compressed, output_buffer, MAX_INPUT_SIZE);
    
    if (decompressed_size != input_len || 
        memcmp(input_buffer, output_buffer, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}