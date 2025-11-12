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

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output->size + count > MAX_OUTPUT_SIZE) {
                return -1;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output->data[output->size++] = value;
            }
            i += 3;
        } else {
            output->data[output->size++] = input[i];
            i++;
        }
    }
    
    return 0;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input data (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    size_t input_size = 0;
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1 || byte > 0xFF) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        input_data[input_size++] = (uint8_t)byte;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (input_size != decompressed.size || 
        memcmp(input_data, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    printf("\nCompression successful. Ratio: %.2f%%\n", 
           (double)compressed.size / input_size * 100.0);
    
    return 0;
}