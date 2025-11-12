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
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input == NULL || input_size == 0) return output;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && 
               count < UCHAR_MAX && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 2 || current == 0xFF) {
            if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = (uint8_t)count;
            output.data[out_idx++] = current;
        } else {
            for (size_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
                output.data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input == NULL || input_size == 0) return output;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < MAX_OUTPUT_SIZE) {
        if (input[in_idx] == 0xFF && in_idx + 2 < input_size) {
            uint8_t count = input[in_idx + 1];
            uint8_t value = input[in_idx + 2];
            
            if (out_idx + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t i = 0; i < count; i++) {
                output.data[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output.data[out_idx++] = input[in_idx++];
        }
    }
    
    output.size = out_idx;
    return output;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input_str);
    printf("Original hex: ");
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X ", (uint8_t)input_str[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data((uint8_t*)input_str, input_len);
    printf("Compressed (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    printf("Decompressed (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input_len && 
        memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
        return 1;
    }
    
    return 0;
}