//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) break;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input[MAX_INPUT_SIZE * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    buffer->size = len;
    memcpy(buffer->data, input, len);
    return 1;
}

void display_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&input_data)) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    display_hex(input_data.data, input_data.size);
    
    if (!validate_input(input_data.data, input_data.size)) {
        printf("Error: Invalid input data\n");
        return 1;
    }
    
    compressed.size = compress_rle(input_data.data, input_data.size, 
                                 compressed.data, MAX_OUTPUT_SIZE);
    
    if (compressed.size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    display_hex(compressed.data, compressed.size);
    
    decompressed.size = decompress_rle(compressed.data, compressed.size,
                                     decompressed.data, MAX_INPUT_SIZE);
    
    if (decompressed.size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    display_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_data.size && 
        memcmp(decompressed.data, input_data.data, input_data.size) == 0) {
        printf("\nSuccess: Data verified correctly\n");
        double ratio = (double)compressed.size / input_data.size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nError: Data verification failed\n");
        return 1;
    }
    
    return 0;
}