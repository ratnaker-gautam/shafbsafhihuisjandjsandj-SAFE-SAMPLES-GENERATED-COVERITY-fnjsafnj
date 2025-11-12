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
    if (input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
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
        else if (i + 1 < buf->size) printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int get_user_input(Buffer* buf) {
    if (buf == NULL) return 0;
    
    printf("Enter data to compress (hex digits, max %zu bytes): ", MAX_INPUT_SIZE);
    
    char input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len % 2 != 0 || len > MAX_INPUT_SIZE * 2) return 0;
    
    buf->size = len / 2;
    
    for (size_t i = 0; i < buf->size; i++) {
        char hex[3] = {input[i * 2], input[i * 2 + 1], '\0'};
        
        if (!isxdigit(hex[0]) || !isxdigit(hex[1])) return 0;
        
        unsigned int value;
        if (sscanf(hex, "%02x", &value) != 1) return 0;
        buf->data[i] = (uint8_t)value;
    }
    
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("RLE Compression/Decompression Tool\n");
    
    if (!get_user_input(&input)) {
        printf("Error: Invalid input format\n");
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
    
    if (input.size == decompressed.size && 
        memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("\nVerification: SUCCESS - Data matches original\n");
        double ratio = (double)compressed.size / input.size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nVerification: FAILED - Data mismatch\n");