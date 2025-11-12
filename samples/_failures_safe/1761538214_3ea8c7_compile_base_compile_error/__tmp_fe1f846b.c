//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, CompressedData* output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < compressed->size) {
        if (out_idx >= output_size) return 0;
        
        if (compressed->data[i] == 0xFF && i + 2 < compressed->size) {
            uint8_t value = compressed->data[i + 1];
            uint8_t count = compressed->data[i + 2];
            
            if (out_idx + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            i += 3;
        } else {
            output[out_idx++] = compressed->data[i++];
        }
    }
    
    return out_idx;
}

int compare_data(const uint8_t* data1, const uint8_t* data2, size_t len) {
    if (data1 == NULL || data2 == NULL) return 0;
    for (size_t i = 0; i < len; i++) {
        if (data1[i] != data2[i]) return 0;
    }
    return 1;
}

void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input_data, input_buffer, input_len);
    
    printf("\nOriginal data (%zu bytes):\n", input_len);
    print_hex(input_data, input_len);
    
    size_t compressed_size = simple_compress(input_data, input_len, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    size_t decompressed_size = simple_decompress(&compressed, decompressed, sizeof(decompressed));
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if (decompressed_size != input_len || !compare_data(input