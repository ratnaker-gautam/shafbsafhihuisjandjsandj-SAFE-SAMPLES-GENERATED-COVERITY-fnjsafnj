//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer compress_rle(const uint8_t* input, size_t input_size) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0) {
        return result;
    }
    
    uint8_t* output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size && output_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output_index + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        output[output_index++] = (uint8_t)count;
        output[output_index++] = current;
        i += count;
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

Buffer decompress_rle(const uint8_t* input, size_t input_size) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || input_size % 2 != 0) {
        return result;
    }
    
    uint8_t* output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_size && output_index < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_index + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count && output_index < MAX_OUTPUT_SIZE; j++) {
            output[output_index++] = value;
        }
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 3];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too long\n");
        return 1;
    }
    
    memcpy(input_buffer, input_str, input_len);
    
    printf("\nOriginal data (%zu bytes):\n", input_len);
    print_buffer_hex(input_buffer, input_len);
    
    Buffer compressed = compress_rle(input_buffer, input_len);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input_buffer, input_len) == 0) {
        printf("\nVerification: SUCCESS\n");
    } else {
        printf("\nVerification: FAILED\n");
    }
    
    printf("\nDecompressed text: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        if (isprint(decompressed.data[i])) {
            printf("%c", decompressed.data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}