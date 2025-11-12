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
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[(*output_size)++] = value;
        }
        
        i += 2;
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    
    if (compress_rle(input_data, input_len, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    
    if (decompress_rle(&compressed, decompressed, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}