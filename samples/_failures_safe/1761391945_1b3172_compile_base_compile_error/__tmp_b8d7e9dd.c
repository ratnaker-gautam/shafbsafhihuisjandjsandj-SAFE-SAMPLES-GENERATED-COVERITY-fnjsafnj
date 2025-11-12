//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 < output_max) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 2 >= output_max) break;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_max) {
        uint8_t current = input[in_idx++];
        
        if (in_idx < input_len && input[in_idx] > 1 && input[in_idx] <= 255) {
            size_t count = input[in_idx++];
            size_t i;
            for (i = 0; i < count && out_idx < output_max; i++) {
                output[out_idx++] = current;
            }
        } else {
            if (out_idx < output_max) {
                output[out_idx++] = current;
            }
        }
    }
    
    return out_idx;
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, (int)max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return validate_input((char*)buffer, len) ? (int)len : 0;
}

void display_buffer(const char* label, const uint8_t* data, size_t len) {
    if (label == NULL || data == NULL) return;
    
    printf("%s: ", label);
    size_t i;
    for (i = 0; i < len && i < 50; i++) {
        if (data[i] >= 32 && data[i] <= 126) {
            printf("%c", data[i]);
        } else {
            printf("[0x%02X]", data[i]);
        }
    }
    if (len > 50) printf("...");
    printf(" (%zu bytes)\n", len);
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    int input_len = get_user_input(input, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        printf("Invalid input or input too large\n");
        return 1;
    }
    
    printf("\nOriginal data:\n");
    display_buffer("Input", input, (size_t)input_len);
    
    size_t compressed_len = simple_compress(input, (size_t)input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nAfter compression:\n");
    display_buffer("Compressed", compressed, compressed_len);
    
    double ratio = (double)compressed_len / (double)input_len * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    size_t decompressed_len = simple_decompress(compressed, compressed_len, decompressed, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nAfter decompression:\n");
    display_buffer("Decompressed", decompressed, decompressed_len);
    
    if (decompressed_len == (size_t)input_len && 
        memcmp(input, decompressed, decompressed_len) ==