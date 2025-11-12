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

Buffer compress_rle(const uint8_t *input, size_t input_size) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0) return result;
    
    size_t max_output_size = input_size * 2;
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) return result;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (output_index + 2 > max_output_size) {
            free(output);
            return result;
        }
        
        output[output_index++] = (uint8_t)count;
        output[output_index++] = current;
        i += count;
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

Buffer decompress_rle(const uint8_t *input, size_t input_size) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || input_size % 2 != 0) return result;
    
    size_t max_output_size = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (input[i] == 0) return result;
        if (max_output_size + input[i] < max_output_size) {
            return result;
        }
        max_output_size += input[i];
    }
    
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) return result;
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (output_index >= max_output_size) {
                free(output);
                return result;
            }
            output[output_index++] = value;
        }
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[256];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = 0;
    for (size_t i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
        if (input_len >= MAX_INPUT_SIZE - 1) break;
        input[input_len++] = (uint8_t)line[i];
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed = compress_rle(input, input_len);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.size) / input_len * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input_len && 
        memcmp(input, decompressed.data, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}