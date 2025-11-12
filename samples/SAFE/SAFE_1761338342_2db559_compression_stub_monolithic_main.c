//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 1 || current >= 128) {
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
        } else {
            output->data[output->size++] = current;
        }
        
        i += count;
    }
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_INPUT_SIZE) {
        if (input->data[in_idx] == 0xFF && in_idx + 2 < input->size) {
            uint8_t value = input->data[in_idx + 1];
            uint8_t count = input->data[in_idx + 2];
            
            if (count == 0) {
                in_idx += 3;
                continue;
            }
            
            for (uint8_t j = 0; j < count && out_idx < MAX_INPUT_SIZE; j++) {
                output[out_idx++] = value;
            }
            in_idx += 3;
        } else {
            output[out_idx++] = input->data[in_idx++];
        }
    }
    
    *output_size = out_idx;
}

int validate_input(const char* str, size_t max_len) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        if (!isprint((unsigned char)str[len])) {
            return 0;
        }
        len++;
    }
    
    return len > 0 && len < max_len;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(input_data, input_str, input_len);
    
    compress_rle(input_data, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    decompress_rle(&compressed, output_data, &decompressed_size);
    
    if (decompressed_size != input_len || 
        memcmp(input_data, output_data, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: %s\n", input_str);
    
    output_data[decompressed_size] = '\0';
    printf("Decompressed: %s\n", output_data);
    
    return 0;
}