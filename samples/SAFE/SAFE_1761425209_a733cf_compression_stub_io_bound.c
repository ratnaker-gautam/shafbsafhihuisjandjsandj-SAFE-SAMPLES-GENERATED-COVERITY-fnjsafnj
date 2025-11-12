//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int main() {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char temp_buf[MAX_INPUT_SIZE + 2];
    if (fgets(temp_buf, sizeof(temp_buf), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(temp_buf);
    if (len > 0 && temp_buf[len - 1] == '\n') {
        temp_buf[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_buf, len);
    input_buf.size = len;
    
    if (!validate_input(input_buf.data, input_buf.size)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Original data: ");
    for (size_t i = 0; i < input_buf.size; i++) {
        if (isprint(input_buf.data[i])) {
            putchar(input_buf.data[i]);
        } else {
            printf("[0x%02X]", input_buf.data[i]);
        }
    }
    printf("\n");
    
    compressed_buf.size = simple_compress(input_buf.data, input_buf.size, 
                                        compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buf.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.size / input_buf.size) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_buf.size; i++) {
        printf("%02X ", compressed_buf.data[i]);
    }
    printf("\n");
    
    decompressed_buf.size = simple_decompress(compressed_buf.data, compressed_buf.size,
                                            decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_buf.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed_buf.size != input_buf.size || 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.size) != 0) {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed successfully\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_buf.size; i++) {
        if (isprint(decompressed_buf.data[i])) {
            putchar(decompressed_buf.data[i]);
        } else {
            printf("[0x%02X]", decompressed_buf.data[i]);
        }
    }
    printf("\n");
    
    return 0;
}