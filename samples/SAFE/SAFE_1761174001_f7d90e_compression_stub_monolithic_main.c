//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        if (out_idx >= MAX_OUTPUT_SIZE - 2) {
            output.size = 0;
            return output;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output.data[out_idx++] = 0xFF;
            if (out_idx >= MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            output.data[out_idx++] = current;
            if (out_idx >= MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            output.data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= MAX_OUTPUT_SIZE) {
                    output.size = 0;
                    return output;
                }
                output.data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        if (input[in_idx] == 0xFF && in_idx + 2 < input_size) {
            uint8_t value = input[in_idx + 1];
            uint8_t count = input[in_idx + 2];
            
            if (out_idx + count > MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output.data[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            output.data[out_idx++] = input[in_idx++];
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input_data, input_str, input_len);
    size_t input_size = input_len;
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%c", input_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(input_data, input_size);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_size * 100.0f);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0 || decompressed.size != input_size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    
    return 0;
}