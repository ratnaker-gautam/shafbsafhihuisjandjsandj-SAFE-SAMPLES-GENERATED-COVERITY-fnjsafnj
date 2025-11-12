//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i = j;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
}

int main() {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    memcpy(input_data.data, temp_input, input_len);
    input_data.size = input_len;
    
    compress_run_length(input_data.data, input_data.size, &compressed);
    
    printf("Original size: %zu bytes\n", input_data.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_data.size - compressed.size) * 100.0f / input_data.size);
        
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02X ", compressed.data[i]);
        }
        printf("\n");
        
        decompress_run_length(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == input_data.size && 
            memcmp(decompressed.data, input_data.data, input_data.size) == 0) {
            printf("Decompression successful: %s\n", decompressed.data);
        } else {
            printf("Decompression failed\n");
            return 1;
        }
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}