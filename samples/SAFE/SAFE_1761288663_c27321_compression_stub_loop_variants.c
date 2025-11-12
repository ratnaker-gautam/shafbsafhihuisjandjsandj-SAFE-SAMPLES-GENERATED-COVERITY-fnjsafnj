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
    if (output == NULL) return 0;
    
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
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t len, uint8_t* output) {
    if (output == NULL || len % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < len; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        size_t k = 0;
        do {
            output[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    return out_idx;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE + 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    temp[strcspn(temp, "\n")] = 0;
    
    size_t input_len = strlen(temp);
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input.data, temp, input_len);
    input.length = input_len;
    
    printf("\nOriginal data (%zu bytes):\n", input.length);
    print_buffer_hex(input.data, input.length);
    
    compressed.length = simple_compress(input.data, input.length, compressed.data);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.length);
    print_buffer_hex(compressed.data, compressed.length);
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.length / input.length * 100.0f);
    
    decompressed.length = simple_decompress(compressed.data, compressed.length, decompressed.data);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.length);
    print_buffer_hex(decompressed.data, decompressed.length);
    
    if (decompressed.length == input.length && 
        memcmp(decompressed.data, input.data, input.length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}