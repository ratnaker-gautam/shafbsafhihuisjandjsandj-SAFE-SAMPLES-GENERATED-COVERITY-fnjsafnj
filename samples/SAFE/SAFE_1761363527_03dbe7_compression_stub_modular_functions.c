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
    for (size_t i = 0; i < length; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

CompressedData compress_rle(const char* input, size_t length) {
    CompressedData result = {0};
    if (length == 0) return result;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < length && out_index < MAX_OUTPUT_SIZE - 2) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < length && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_index + 2 > MAX_OUTPUT_SIZE) break;
        
        result.data[out_index++] = count;
        result.data[out_index++] = (uint8_t)current;
        i += count;
    }
    
    result.size = out_index;
    return result;
}

void decompress_rle(const CompressedData* compressed, char* output, size_t max_output) {
    if (compressed == NULL || output == NULL) return;
    if (max_output == 0) return;
    
    size_t out_index = 0;
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 >= compressed->size) break;
        
        uint8_t count = compressed->data[i];
        char character = (char)compressed->data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (out_index >= max_output - 1) break;
            output[out_index++] = character;
        }
    }
    output[out_index] = '\0';
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    char output[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
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
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    CompressedData compressed = compress_rle(input, input_len);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.size) / input_len * 100);
    
    decompress_rle(&compressed, output, sizeof(output));
    
    if (strcmp(input, output) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}