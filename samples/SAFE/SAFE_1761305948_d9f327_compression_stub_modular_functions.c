//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    
    if (input->length == 0) {
        output->length = 0;
        return 1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->length && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->length && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->length = out_idx;
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    
    if (input->length == 0) {
        output->length = 0;
        return 1;
    }
    
    if (input->length % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->length; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->length = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int get_user_input(Buffer* input) {
    if (input == NULL) {
        return 0;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return 0;
    }
    
    memcpy(input->data, temp, len);
    input->length = len;
    return 1;
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&original)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", original.length);
    print_buffer_hex(&original);
    
    if (!compress_rle(&original, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.length);
    print_buffer_hex(&compressed);
    
    if (!decompress_rle(&compressed, &decompressed)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.length);
    print_buffer_hex(&decompressed);
    
    if (original.length != decompressed.length) {
        printf("Error: Size mismatch\n");
        return 1;
    }
    
    if (memcmp(original.data, decompressed.data, original.length) != 0) {
        printf("Error: Data mismatch\n");
        return 1;
    }
    
    printf("Compression successful. Ratio: %.2f%%\n", 
           (float)compressed.length / original.length * 100.0f);
    
    return 0;
}