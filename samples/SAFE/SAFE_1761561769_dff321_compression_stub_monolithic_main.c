//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 127 && input[i + count] == current) {
            count++;
        }
        
        if (count > 1) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_size && input[i] != input[i + 1] && (i - literal_start) < 127) {
                i++;
            }
            
            size_t literal_length = i - literal_start;
            if (literal_length > 0) {
                output->data[output->size++] = (uint8_t)(0x80 | literal_length);
                for (size_t j = 0; j < literal_length && output->size < MAX_OUTPUT_SIZE; j++) {
                    output->data[output->size++] = input[literal_start + j];
                }
            }
        }
    }
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    while (i < input->size && *output_size < MAX_INPUT_SIZE) {
        uint8_t flag = input->data[i++];
        
        if (flag & 0x80) {
            size_t literal_length = flag & 0x7F;
            if (literal_length == 0 || i + literal_length > input->size) {
                break;
            }
            
            for (size_t j = 0; j < literal_length && *output_size < MAX_INPUT_SIZE; j++) {
                output[(*output_size)++] = input->data[i++];
            }
        } else {
            size_t run_length = flag;
            if (run_length == 0 || i >= input->size) {
                break;
            }
            
            uint8_t value = input->data[i++];
            for (size_t j = 0; j < run_length && *output_size < MAX_INPUT_SIZE; j++) {
                output[(*output_size)++] = value;
            }
        }
    }
}

int main() {
    uint8_t original[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, sizeof(original), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)original);
    if (input_size > 0 && original[input_size - 1] == '\n') {
        original[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(original, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_size - compressed.size) / input_size * 100.0f);
        
        decompress_rle(&compressed, decompressed, &decompressed_size);
        
        if (decompressed_size == input_size && 
            memcmp(original, decompressed, input_size) == 0) {
            printf("Decompression successful - data verified\n");
        } else {
            printf("Decompression failed - data mismatch\n");
            return 1;
        }
        
        printf("Compressed data (hex): ");
        for (size_t i = 0; i < compressed.size && i < 32; i++) {
            printf("%02X ", compressed.data[i]);
        }
        if (compressed.size > 32) {
            printf("...");
        }
        printf("\n");
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}