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

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
    
    return 0;
}

int main(void) {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_text[MAX_INPUT_SIZE];
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    input_buf.size = input_len;
    memcpy(input_buf.data, input_text, input_len);
    
    if (compress_rle(input_buf.data, input_buf.size, &compressed_buf) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.size / input_buf.size) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_buf.size; i++) {
        printf("%02X ", compressed_buf.data[i]);
    }
    printf("\n");
    
    if (decompress_rle(compressed_buf.data, compressed_buf.size, &decompressed_buf) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_buf.size != input_buf.size || 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed_buf.size; i++) {
        printf("%c", decompressed_buf.data[i]);
    }
    printf("\n");
    
    printf("Compression/decompression successful\n");
    
    return 0;
}