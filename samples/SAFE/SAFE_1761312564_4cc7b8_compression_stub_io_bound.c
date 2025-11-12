//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 3 > output_size) return 0;
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
        } else {
            if (output_idx + count > output_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_idx + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx >= output_size) return 0;
            output[output_idx++] = input[i];
            i++;
        }
    }
    
    return output_idx;
}

int main() {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t pos = 0;
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE - 1) {
        input_buffer.data[pos++] = (uint8_t)c;
    }
    input_buffer.size = pos;
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data size: %zu bytes\n", input_buffer.size);
    
    size_t compressed_size = simple_compress(input_buffer.data, input_buffer.size, 
                                           compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_buffer.size) * 100);
    
    size_t decompressed_size = simple_decompress(compressed_buffer.data, compressed_size,
                                                decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size != input_buffer.size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input_buffer.data, decompressed_buffer.data, input_buffer.size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size && i < 50; i++) {
        if (isprint(decompressed_buffer.data[i])) {
            putchar(decompressed_buffer.data[i]);
        } else {
            printf("\\x%02X", decompressed_buffer.data[i]);
        }
    }
    if (decompressed_size > 50) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}