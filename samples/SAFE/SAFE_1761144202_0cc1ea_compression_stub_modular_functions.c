//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    const char* test_string = "AAAABBBCCDAA";
    size_t input_size = strlen(test_string);
    
    if (input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(original, test_string, input_size);
    
    printf("Original data (%zu bytes):\n", input_size);
    print_buffer_hex(original, input_size);
    printf("As string: %.*s\n\n", (int)input_size, original);
    
    if (!validate_input(original, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t compressed_size = compress_rle(original, input_size, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed, compressed_size);
    printf("Compression ratio: %.2f%%\n\n", 
           (float)(input_size - compressed_size) * 100.0f / input_size);
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(decompressed, decompressed_size);
    printf("As string: %.*s\n\n", (int)decompressed_size, decompressed);
    
    if (decompressed_size != input_size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (!compare_buffers(original, decompressed, input_size)) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful!\n");
    return 0;
}