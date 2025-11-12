//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input_size && j < i + 255; j++) {
            if (input[j] != current) {
                break;
            }
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i = j;
    }
}

void decompress_run_length(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    do {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        size_t k = 0;
        while (k < count) {
            if (*output_size >= MAX_INPUT_SIZE) {
                return;
            }
            output[*output_size] = value;
            (*output_size)++;
            k++;
        }
        
        i += 2;
    } while (i < input->size);
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_size < MAX_INPUT_SIZE - 1) {
            input_data[input_size] = (uint8_t)c;
            input_size++;
        }
    }
    input_data[input_size] = '\0';
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    compress_run_length(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_size - compressed.size) * 100.0f / (float)input_size);
    }
    
    decompress_run_length(&compressed, output_data, &decompressed_size);
    
    if (decompressed_size == input_size && 
        memcmp(input_data, output_data, input_size) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    for (size_t idx = 0; idx < compressed.size; idx++) {
        printf("%02X ", compressed.data[idx]);
    }
    printf("\n");
    
    return 0;
}