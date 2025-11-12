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
    if (input == NULL || input_size == 0) {
        return result;
    }
    
    size_t max_output_size = input_size * 2;
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) {
        return result;
    }
    
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
    if (input == NULL || input_size == 0 || input_size % 2 != 0) {
        return result;
    }
    
    size_t max_output_size = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (i + 1 >= input_size) break;
        max_output_size += input[i + 1];
        if (max_output_size > MAX_INPUT_SIZE * 10) {
            return result;
        }
    }
    
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) {
        return result;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        if (i + 1 >= input_size) break;
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output_index + count > max_output_size) {
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
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed = compress_rle((uint8_t*)input_str, input_len);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        if (isprint(compressed.data[i])) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[0x%02X]", compressed.data[i]);
        }
    }
    printf("\n");
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    printf("Original size: %zu, Compressed size: %zu, Ratio: %.2f\n",
           input_len, compressed.size, (float)compressed.size / input_len);
    
    int verify = memcmp(input_str, decompressed.data, input_len);
    if (verify == 0 && decompressed.size == input_len) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}