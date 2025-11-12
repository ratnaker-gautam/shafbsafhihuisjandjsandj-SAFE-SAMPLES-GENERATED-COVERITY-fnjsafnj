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
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) break;
        
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
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp_input[MAX_INPUT_SIZE + 1];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    temp_input[strcspn(temp_input, "\n")] = 0;
    
    size_t input_len = strlen(temp_input);
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!validate_input((uint8_t*)temp_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    printf("\nOriginal data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    compressed_buf.length = simple_compress(input_buf.data, input_buf.length, compressed_buf.data);
    
    if (compressed_buf.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_buf.length);
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    decompressed_buf.length = simple_decompress(compressed_buf.data, compressed_buf.length, decompressed_buf.data);
    
    if (decompressed_buf.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_buf.length);
    print_buffer_hex(decompressed_buf.data, decompressed_buf.length);
    
    if (decompressed_buf.length == input_buf.length && 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.length) == 0) {
        printf("\nVerification: SUCCESS - Data matches original\n");
        
        double ratio = (double)compressed_buf.length / input_buf.length * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nVerification: FAILED - Data corrupted\n");
        return 1;
    }
    
    return 0;
}