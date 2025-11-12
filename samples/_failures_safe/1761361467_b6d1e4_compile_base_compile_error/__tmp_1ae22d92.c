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
        
        if (count >= 3 || current == 0xFF) {
            output[output_index++] = 0xFF;
            output[output_index++] = current;
            output[output_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output_index < MAX_OUTPUT_SIZE; j++) {
                output[output_index++] = current;
            }
        }
        
        i += count;
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

Buffer decompress_rle(const uint8_t* input, size_t input_size) {
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
    
    while (i < input_size && output_index < MAX_OUTPUT_SIZE) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count == 0) {
                free(output);
                return result;
            }
            
            for (uint8_t j = 0; j < count && output_index < MAX_OUTPUT_SIZE; j++) {
                output[output_index++] = value;
            }
            
            i += 3;
        } else {
            output[output_index++] = input[i++];
        }
    }
    
    result.data = output;
    result.size = output_index;
    return result;
}

void print_hex(const uint8_t* data, size_t size) {
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

int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, (int)max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return (int)len;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    int input_size = get_user_input(input_buffer, MAX_INPUT_SIZE);
    
    if (input_size <= 0) {
        printf("No input received or input error.\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    print_hex(input_buffer, (size_t)input_size);
    
    Buffer compressed = compress_rle(input_buffer, (size_t)input_size);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        free(compressed.data);
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == (size_t)input_size && 
        memcmp(input_buffer, decompressed.data, decompressed.size) == 0) {
        printf("\nCompression and decompression successful!\n");
    } else {
        printf("\nError: Decompressed data doesn't match original!\n");
    }
    
    printf("\nOriginal text: %s\n", input_buffer);
    printf("Decompressed text