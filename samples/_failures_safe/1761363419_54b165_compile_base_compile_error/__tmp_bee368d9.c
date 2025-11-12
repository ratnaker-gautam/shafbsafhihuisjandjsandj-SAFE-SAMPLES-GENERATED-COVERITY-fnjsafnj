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

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_rle_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t max_output) {
    if (input == NULL || output == NULL || input_len == 0 || max_output == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx + 2 <= max_output) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    if (i < input_len) return 0;
    return output_idx;
}

size_t simple_rle_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t max_output) {
    if (input == NULL || output == NULL || input_len == 0 || max_output == 0) return 0;
    if (input_len % 2 != 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < max_output) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (output_idx + count > max_output) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
    }
    
    return output_idx;
}

int compress_data(const char* input, size_t input_len, CompressedData* compressed) {
    if (!validate_input(input, input_len) || compressed == NULL) return 0;
    
    size_t compressed_size = simple_rle_compress((const uint8_t*)input, input_len, 
                                                compressed->data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) return 0;
    
    compressed->size = compressed_size;
    return 1;
}

int decompress_data(const CompressedData* compressed, char* output, size_t max_output) {
    if (compressed == NULL || output == NULL || max_output == 0) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t decompressed_size = simple_rle_decompress(compressed->data, compressed->size,
                                                    (uint8_t*)output, max_output);
    
    if (decompressed_size == 0) return 0;
    
    output[decompressed_size] = '\0';
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    char output_buffer[MAX_INPUT_SIZE + 1];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!compress_data(input_buffer, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    if (!decompress_data(&compressed, output_buffer, sizeof(output_buffer))) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed text: %s\n", output_buffer);
    
    if (strcmp(input_buffer, output_buffer) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Data mismatch error\n");
        return 1;
    }
    
    double ratio = (double)compressed.size / input_len * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return