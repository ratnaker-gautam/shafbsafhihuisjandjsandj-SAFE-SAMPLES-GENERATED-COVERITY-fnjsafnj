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
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) return 0;
        
        output->data[out_idx++] = count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return 1;
}

int decompress_rle(const CompressedData* compressed, char* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < compressed->size) {
        if (in_idx + 1 >= compressed->size) return 0;
        
        uint8_t count = compressed->data[in_idx++];
        uint8_t value = compressed->data[in_idx++];
        
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

int main(void) {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_rle(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    print_compressed(&compressed);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_rle(&compressed, output, sizeof(output))) {
        fprintf(stderr, "Decompression failed\n");
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