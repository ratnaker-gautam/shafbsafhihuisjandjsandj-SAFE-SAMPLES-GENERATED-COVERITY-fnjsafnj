//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size == 0) {
        output->size = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        i = j;
    }
    
    output->size = out_idx;
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size % 2 != 0) {
        output->size = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    for (; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    output->size = out_idx;
}

int main(void) {
    uint8_t original[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t original_size = sizeof(original);
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    compress_run_length(original, original_size, &compressed);
    
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu\n", original_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f\n", (float)compressed.size / original_size);
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed.size != original_size) {
        printf("Size mismatch after decompression\n");
        return 1;
    }
    
    int match = 1;
    size_t idx = 0;
    while (idx < original_size) {
        if (decompressed.data[idx] != original[idx]) {
            match = 0;
            break;
        }
        idx++;
    }
    
    if (match) {
        printf("Decompression successful - data matches original\n");
    } else {
        printf("Decompression failed - data mismatch\n");
        return 1;
    }
    
    printf("Sample compressed data: ");
    size_t print_count = compressed.size > 10 ? 10 : compressed.size;
    size_t p = 0;
    while (p < print_count) {
        printf("%d ", compressed.data[p]);
        p++;
    }
    printf("\n");
    
    return 0;
}