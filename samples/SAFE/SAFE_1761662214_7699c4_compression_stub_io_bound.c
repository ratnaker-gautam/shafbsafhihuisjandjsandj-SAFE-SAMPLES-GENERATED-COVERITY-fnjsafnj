//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size == 0) {
        return -1;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            return -1;
        }
        
        uint8_t count = input->data[i++];
        uint8_t value = input->data[i++];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[(*output_size)++] = value;
        }
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE - 1) {
        input_data[input_size++] = (uint8_t)c;
    }
    input_data[input_size] = '\0';
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_size) * 100.0);
    
    if (decompress_rle(&compressed, output_data, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed_size != input_size || 
        memcmp(input_data, output_data, input_size) != 0) {
        printf("Verification failed.\n");
        return 1;
    }
    
    printf("Decompression verified successfully.\n");
    printf("Original: ");
    for (size_t i = 0; i < (input_size > 50 ? 50 : input_size); i++) {
        if (isprint(input_data[i])) {
            putchar(input_data[i]);
        } else {
            putchar('.');
        }
    }
    printf("\n");
    
    printf("Decompressed: ");
    for (size_t i = 0; i < (decompressed_size > 50 ? 50 : decompressed_size); i++) {
        if (isprint(output_data[i])) {
            putchar(output_data[i]);
        } else {
            putchar('.');
        }
    }
    printf("\n");
    
    return 0;
}