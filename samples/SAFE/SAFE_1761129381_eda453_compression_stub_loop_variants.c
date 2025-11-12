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
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
        output->size += 2;
        i += count;
    }
}

void decompress_run_length(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size == 0) {
        *output_size = 0;
        return;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output[*output_size + k] = value;
            k++;
        } while (k < count);
        
        *output_size += count;
        i += 2;
    }
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE - 1) {
        input_data[input_size++] = (uint8_t)c;
    }
    input_data[input_size] = '\0';
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    compress_run_length(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100.0f);
    
    decompress_run_length(&compressed, decompressed, &decompressed_size);
    
    if (decompressed_size != input_size || 
        memcmp(input_data, decompressed, input_size) != 0) {
        printf("Decompression verification failed!\n");
        return 1;
    }
    
    printf("Decompression verified successfully.\n");
    printf("Original: ");
    
    size_t idx = 0;
    while (idx < input_size && idx < 50) {
        putchar(input_data[idx]);
        idx++;
    }
    if (input_size > 50) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}