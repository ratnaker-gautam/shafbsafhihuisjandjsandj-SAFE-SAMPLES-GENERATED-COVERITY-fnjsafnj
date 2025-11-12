//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

void compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    output->size = out_idx;
}

void decompress_rle(const CompressedData* input, uint8_t* output, size_t* output_size) {
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count >= MAX_INPUT_SIZE) {
            break;
        }
        
        size_t j = 0;
        do {
            output[out_idx++] = value;
            j++;
        } while (j < count);
    }
    
    *output_size = out_idx;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE - 1) {
        if (isprint(c)) {
            input_data[input_size++] = (uint8_t)c;
        }
    }
    input_data[input_size] = '\0';
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memset(&compressed, 0, sizeof(compressed));
    compress_rle(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        double ratio = (double)compressed.size / input_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    memset(output_data, 0, sizeof(output_data));
    decompress_rle(&compressed, output_data, &decompressed_size);
    
    if (decompressed_size == input_size && memcmp(input_data, output_data, input_size) == 0) {
        printf("Decompression successful\n");
        printf("Original: %s\n", input_data);
        printf("Decompressed: %s\n", output_data);
    } else {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    return 0;
}