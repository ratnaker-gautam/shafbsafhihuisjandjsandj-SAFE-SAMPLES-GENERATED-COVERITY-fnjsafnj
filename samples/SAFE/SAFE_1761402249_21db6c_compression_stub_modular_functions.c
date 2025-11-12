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

int compress_rle(const char* input, size_t input_len, CompressedData* output) {
    if (output == NULL) return 0;
    output->size = 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const CompressedData* compressed, char* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 >= compressed->size) return 0;
        
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    output[out_idx] = '\0';
    return 1;
}

void print_compressed(const CompressedData* compressed) {
    if (compressed == NULL) return;
    printf("Compressed data (%zu bytes): ", compressed->size);
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02X ", compressed->data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_rle(input, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    print_compressed(&compressed);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    if (!decompress_rle(&compressed, output, sizeof(output))) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", output);
    
    if (strcmp(input, output) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}