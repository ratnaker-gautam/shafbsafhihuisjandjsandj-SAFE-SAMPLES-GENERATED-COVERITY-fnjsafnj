//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int validate_input(const char* input, size_t input_len) {
    if (input == NULL) {
        return 0;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

CompressedData compress_rle(const char* input, size_t input_len) {
    CompressedData result = {0};
    
    if (!validate_input(input, input_len)) {
        return result;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_len && output_index < MAX_OUTPUT_SIZE - 2; ) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (output_index + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        result.data[output_index++] = count;
        result.data[output_index++] = (uint8_t)current;
        
        i += count;
    }
    
    result.size = output_index;
    return result;
}

void decompress_rle(const CompressedData* compressed, char* output, size_t output_size) {
    if (compressed == NULL || output == NULL || output_size == 0) {
        return;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < compressed->size && output_index < output_size - 1; i += 2) {
        if (i + 1 >= compressed->size) {
            break;
        }
        
        uint8_t count = compressed->data[i];
        char character = (char)compressed->data[i + 1];
        
        for (uint8_t j = 0; j < count && output_index < output_size - 1; j++) {
            output[output_index++] = character;
        }
    }
    
    output[output_index] = '\0';
}

void print_compressed(const CompressedData* compressed) {
    if (compressed == NULL) {
        return;
    }
    
    printf("Compressed data (%zu bytes): ", compressed->size);
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02X ", compressed->data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    
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
    
    CompressedData compressed = compress_rle(input, input_len);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    print_compressed(&compressed);
    
    decompress_rle(&compressed, output, sizeof(output));
    
    printf("Decompressed: %s\n", output);
    
    if (strcmp(input, output) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}