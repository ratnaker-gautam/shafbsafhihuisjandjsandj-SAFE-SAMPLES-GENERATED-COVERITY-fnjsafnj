//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t len, uint8_t* output) {
    if (output == NULL) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t len, uint8_t* output) {
    if (output == NULL || len % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < len; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        size_t k = 0;
        do {
            output[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    return out_idx;
}

int main(void) {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
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
    
    if (!validate_input((uint8_t*)temp_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    printf("Original: %s\n", input_buf.data);
    printf("Original size: %zu bytes\n", input_buf.length);
    
    compressed_buf.length = simple_compress(input_buf.data, input_buf.length, compressed_buf.data);
    if (compressed_buf.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.length / input_buf.length) * 100.0);
    
    decompressed_buf.length = simple_decompress(compressed_buf.data, compressed_buf.length, decompressed_buf.data);
    if (decompressed_buf.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_buf.length != input_buf.length || 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.length) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", decompressed_buf.data);
    printf("Decompression successful\n");
    
    return 0;
}