//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

int compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size == 0) {
        *output_size = 0;
        return 0;
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
        if (out_idx + 2 > *output_size) return -1;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i = j;
    }
    *output_size = out_idx;
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size % 2 != 0) return -1;

    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        if (out_idx + count > *output_size) return -1;
        size_t k = 0;
        do {
            output[out_idx++] = value;
            k++;
        } while (k < count);
    }
    *output_size = out_idx;
    return 0;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_INPUT_SIZE * 2];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    size_t compressed_size = sizeof(compressed);
    if (compress_rle(input, input_len, compressed, &compressed_size) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_len - compressed_size) / input_len * 100.0f);
    }
    
    printf("Compressed data (hex): ");
    for (size_t idx = 0; idx < compressed_size; idx++) {
        printf("%02X ", compressed[idx]);
    }
    printf("\n");
    
    size_t decompressed_size = sizeof(decompressed);
    if (decompress_rle(compressed, compressed_size, decompressed, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_len || memcmp(input, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed text: ");
    fwrite(decompressed, 1, decompressed_size, stdout);
    printf("\n");
    
    return 0;
}