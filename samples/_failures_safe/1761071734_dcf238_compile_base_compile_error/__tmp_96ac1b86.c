//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, Buffer* output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 3 > MAX_OUTPUT_SIZE) break;
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

size_t simple_decompress(const Buffer* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL || output_size == 0) return 0;
    
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < compressed->size && out_idx < output_size) {
        if (compressed->data[in_idx] == 0xFF && in_idx + 2 < compressed->size) {
            uint8_t value = compressed->data[in_idx + 1];
            uint8_t count = compressed->data[in_idx + 2];
            
            if (out_idx + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output[out_idx++] = compressed->data[in_idx++];
        }
    }
    
    return out_idx;
}

int verify_compression(const uint8_t* original, size_t orig_len, const uint8_t* decompressed, size_t decomp_len) {
    if (original == NULL || decompressed == NULL) return 0;
    if (orig_len != decomp_len) return 0;
    
    for (size_t i = 0; i < orig_len; i++) {
        if (original[i] != decompressed[i]) return 0;
    }
    
    return 1;
}

void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
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
    
    if (input_len > sizeof(input_data)) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_data, input, input_len);
    
    size_t compressed_size = simple_compress(input_data, input_len, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    
    printf("\nCompressed data (hex):\n");
    print_hex(compressed.data, compressed.size