//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input_size && input[j] == current && count < 255; j++) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i = j;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    do {
        if (i + 1 >= input_size) {
            break;
        }
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (output->size >= MAX_OUTPUT_SIZE) {
                return;
            }
            output->data[output->size++] = value;
        }
        
        i += 2;
    } while (i < input_size);
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    compress_run_length(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_size - compressed.size) * 100.0f / input_size);
        
        printf("Compressed data: ");
        for (size_t k = 0; k < compressed.size; k++) {
            printf("%02X ", compressed.data[k]);
        }
        printf("\n");
        
        decompress_run_length(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == input_size && 
            memcmp(input_data, decompressed.data, input_size) == 0) {
            printf("Decompression successful: %s\n", decompressed.data);
        } else {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    return 0;
}