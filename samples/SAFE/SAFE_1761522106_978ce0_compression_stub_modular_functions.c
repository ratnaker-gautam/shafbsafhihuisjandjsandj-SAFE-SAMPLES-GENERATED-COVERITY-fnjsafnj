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
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (!validate_input(input->data, input->size)) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) return 0;
    if (input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int get_user_input(Buffer* input) {
    if (input == NULL) return 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) return 0;
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    memcpy(input->data, temp, len);
    input->size = len;
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&input)) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(&input);
    
    if (!compress_rle(&input, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!decompress_rle(&compressed, &decompressed)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        printf("Error: Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Error: Data mismatch after decompression\n");
        return 1;
    }
    
    printf("\nCompression successful. Ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input.size) * 100.0);
    
    return 0;
}