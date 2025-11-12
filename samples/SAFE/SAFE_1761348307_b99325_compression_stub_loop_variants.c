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
        
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
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
    
    for (i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t j = 0;
        do {
            output->data[output->size] = value;
            output->size++;
            j++;
        } while (j < count);
    }
}

int validate_input(const char* input, size_t* length) {
    if (input == NULL || length == NULL) {
        return 0;
    }
    
    size_t len = 0;
    while (input[len] != '\0') {
        len++;
        if (len > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    
    *length = len;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_length;
    if (!validate_input(input, &input_length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_length > 0 && input[input_length - 1] == '\n') {
        input[input_length - 1] = '\0';
        input_length--;
    }
    
    compress_run_length((uint8_t*)input, input_length, &compressed);
    
    printf("Original size: %zu bytes\n", input_length);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_length > 0) ? (100.0 * compressed.size / input_length) : 0.0);
    
    if (compressed.size > 0) {
        decompress_run_length(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == input_length && 
            memcmp(input, decompressed.data, input_length) == 0) {
            printf("Decompression successful - data verified\n");
        } else {
            printf("Decompression failed - data mismatch\n");
            return 1;
        }
        
        printf("Compressed data (hex): ");
        size_t k = 0;
        while (k < compressed.size) {
            printf("%02X ", compressed.data[k]);
            k++;
        }
        printf("\n");
    } else {
        printf("No data to compress\n");
    }
    
    return 0;
}