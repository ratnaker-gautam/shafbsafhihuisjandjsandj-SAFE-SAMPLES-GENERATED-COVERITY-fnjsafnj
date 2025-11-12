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
    
    for (; i < input_size; i += 2) {
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

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) {
        return 0;
    }
    
    size_t k = 0;
    while (k < size) {
        if (buf1[k] != buf2[k]) {
            return 0;
        }
        k++;
    }
    return 1;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    size_t data_size = 0;
    for (int i = 0; i < 64; i++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original[data_size] = (i % 4) + 'A';
        data_size++;
    }
    
    for (int i = 0; i < 32; i++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original[data_size] = 'X';
        data_size++;
    }
    
    compress_run_length(original, data_size, &compressed);
    
    if (compressed.size > 0) {
        printf("Original size: %zu\n", data_size);
        printf("Compressed size: %zu\n", compressed.size);
        printf("Compression ratio: %.2f\n", (float)compressed.size / data_size);
        
        decompress_run_length(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == data_size && 
            compare_buffers(original, decompressed.data, data_size)) {
            printf("Decompression successful\n");
        } else {
            printf("Decompression failed\n");
        }
    } else {
        printf("Compression failed\n");
    }
    
    return 0;
}