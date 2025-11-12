//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (output_size < input_size * 2) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 > output_size) {
                return 0;
            }
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            if (out_idx + count > output_size) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (out_idx + count > output_size) {
                return 0;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            i += 3;
        } else {
            if (out_idx >= output_size) {
                return 0;
            }
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

int main(void) {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t idx = 0;
    while ((c = getchar()) != EOF && c != '\n' && idx < MAX_INPUT_SIZE) {
        input_buf.data[idx++] = (uint8_t)c;
    }
    input_buf.size = idx;
    
    if (!validate_input(input_buf.data, input_buf.size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    
    compressed_buf.size = simple_compress(input_buf.data, input_buf.size, 
                                        compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buf.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.size / input_buf.size) * 100);
    
    decompressed_buf.size = simple_decompress(compressed_buf.data, compressed_buf.size,
                                            decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_buf.size == 0 || 
        decompressed_buf.size != input_buf.size ||
        memcmp(input_buf.data, decompressed_buf.data, input_buf.size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_buf.size && i < 32; i++) {
        printf("%02X ", compressed_buf.data[i]);
    }
    if (compressed_buf.size > 32) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}