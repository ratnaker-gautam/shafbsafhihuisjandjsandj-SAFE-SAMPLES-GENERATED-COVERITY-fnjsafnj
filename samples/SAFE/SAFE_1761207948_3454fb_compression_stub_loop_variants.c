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

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    do {
        if (i >= input_size) {
            break;
        }
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
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
        input_data[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    compress_run_length(input_data, input_size, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t k = 0; k < compressed.size; k++) {
        printf("%02X ", compressed.data[k]);
    }
    printf("\n");
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == 0 || decompressed.size != input_size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompressed text: ");
    for (size_t m = 0; m < decompressed.size; m++) {
        putchar(decompressed.data[m]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) * 100.0f / (float)input_size);
    
    return 0;
}