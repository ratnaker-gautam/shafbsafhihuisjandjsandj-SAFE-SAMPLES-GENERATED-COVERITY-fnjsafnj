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

size_t compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_idx + 2 > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        for (size_t j = i + 1; j < input_size; j++) {
            if (input[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        output->data[output_idx++] = (uint8_t)count;
        output->data[output_idx++] = current;
        i += count;
    }
    
    output->size = output_idx;
    return output_idx;
}

size_t decompress_rle(const CompressedData* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    do {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i++];
        uint8_t value = input->data[i++];
        
        for (uint8_t j = 0; j < count; j++) {
            if (output_idx >= output_size) {
                return 0;
            }
            output[output_idx++] = value;
        }
    } while (i < input->size);
    
    return output_idx;
}

int main() {
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
    
    size_t compressed_size = compress_rle(input_buffer, input_len, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_len) * 100.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_rle(&compressed, output_buffer, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    output_buffer[decompressed_size] = '\0';
    printf("Decompressed text: %s\n", output_buffer);
    
    if (memcmp(input_buffer, output_buffer, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}