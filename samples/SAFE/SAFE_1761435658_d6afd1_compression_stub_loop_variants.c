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
        
        output->data[output->size] = count;
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
    
    for (i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
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

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    size_t data_size = 0;
    for (size_t i = 0; i < MAX_INPUT_SIZE; i++) {
        original_data[i] = (uint8_t)(i % 256);
        data_size++;
    }
    
    compress_run_length(original_data, data_size, &compressed);
    
    if (compressed.size > 0) {
        decompress_run_length(compressed.data, compressed.size, &decompressed);
    }
    
    int match = 1;
    if (data_size != decompressed.size) {
        match = 0;
    } else {
        for (size_t k = 0; k < data_size; k++) {
            if (original_data[k] != decompressed.data[k]) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Original size: %zu\n", data_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Decompressed size: %zu\n", decompressed.size);
    printf("Data integrity: %s\n", match ? "PASS" : "FAIL");
    
    return 0;
}