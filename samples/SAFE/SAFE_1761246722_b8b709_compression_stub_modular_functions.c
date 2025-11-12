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

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_index + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[out_index++] = (uint8_t)count;
        output->data[out_index++] = current;
        i += count;
    }
    
    output->size = out_index;
    return out_index;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t max_output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0 || compressed->size % 2 != 0) return 0;
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (out_index + count > max_output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    return out_index;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    memcpy(input_data, input_buffer, input_len);
    
    printf("\nOriginal data (%zu bytes):\n", input_len);
    print_hex(input_data, input_len);
    
    size_t compressed_size = simple_compress(input_data, input_len, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed_size / input_len * 100.0f);
    
    size_t decompressed_size = simple_decompress(&compressed, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != input_len) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed, input_len) != 0) {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("\nDecompressed data verified successfully\n");
    printf("Decompressed text: %.*s\n", (int)decompressed_size, decompressed);
    
    return 0;
}