//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t length;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_len, Buffer* output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return;
    }
    
    output->length = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output->length + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < UINT8_MAX) {
            count++;
            j++;
        }
        
        output->data[output->length] = current;
        output->data[output->length + 1] = (uint8_t)count;
        output->length += 2;
        
        i = j;
    }
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input->length == 0) {
        *output_len = 0;
        return;
    }
    
    *output_len = 0;
    size_t i = 0;
    
    for (; i < input->length; i += 2) {
        if (i + 1 >= input->length) {
            break;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (*output_len + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output[*output_len] = value;
            (*output_len)++;
            k++;
        } while (k < count);
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_len;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(input_data, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.length / input_len * 100.0f);
    
    decompress_rle(&compressed, decompressed, &decompressed_len);
    
    if (decompressed_len != input_len) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    int match = 1;
    for (size_t idx = 0; idx < input_len; idx++) {
        if (input_data[idx] != decompressed[idx]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: OK\n");
    } else {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.length; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}