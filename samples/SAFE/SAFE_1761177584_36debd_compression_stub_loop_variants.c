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
        
        output[output_index++] = current;
        output[output_index++] = (uint8_t)count;
        
        i = j;
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
        if (input[i + 1] == 0) return result;
        if (max_output_size > SIZE_MAX - input[i + 1]) {
            return result;
        }
        max_output_size += input[i + 1];
    }
    
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) return result;
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output_index > SIZE_MAX - count) {
            free(output);
            return result;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[256];
    
    printf("Enter text to compress: ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input, line, input_len);
    
    Buffer compressed = compress_rle(input, input_len);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    if (decompressed.size == input_len && memcmp(decompressed.data, input, input_len) == 0) {
        printf("Decompression verified successfully\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}