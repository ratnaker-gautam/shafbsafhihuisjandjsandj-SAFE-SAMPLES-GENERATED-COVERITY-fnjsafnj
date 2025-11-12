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

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input->size == 0 || input->size % 2 != 0) return 0;
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (decompressed_size + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    *output_size = decompressed_size;
    return 1;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_size;
    
    const char* test_string = "AAAABBBCCDAA";
    size_t test_len = strlen(test_string);
    
    if (test_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Test string too long\n");
        return 1;
    }
    
    memcpy(original_data, test_string, test_len);
    
    printf("Original data: %s\n", original_data);
    printf("Original size: %zu bytes\n", test_len);
    
    if (!compress_rle(original_data, test_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / test_len * 100.0f);
    
    if (!decompress_rle(&compressed, decompressed_data, &decompressed_size)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed size: %zu bytes\n", decompressed_size);
    
    if (decompressed_size != test_len) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (!compare_buffers(original_data, decompressed_data, test_len)) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompressed data: %s\n", decompressed_data);
    printf("Compression and decompression successful\n");
    
    return 0;
}