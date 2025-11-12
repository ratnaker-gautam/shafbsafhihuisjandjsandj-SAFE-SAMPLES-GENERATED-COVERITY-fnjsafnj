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

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->length == 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    output->length = out_idx;
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->length == 0 || input->length % 2 != 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->length; i += 2) {
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->length = out_idx;
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(temp_input);
    if (len > 0 && temp_input[len - 1] == '\n') {
        temp_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        input.data[i] = (uint8_t)temp_input[i];
    }
    input.length = len;
    
    printf("Original: ");
    for (size_t i = 0; i < input.length; i++) {
        printf("%c", input.data[i]);
    }
    printf("\n");
    
    if (compress_rle(&input, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    printf("Compression ratio: %.2f\n", (float)compressed.length / input.length);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.length; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (input.length != decompressed.length) {
        fprintf(stderr, "Length mismatch\n");
        return 1;
    }
    
    for (size_t i = 0; i < input.length; i++) {
        if (input.data[i] != decompressed.data[i]) {
            fprintf(stderr, "Data mismatch at position %zu\n", i);
            return 1;
        }
    }
    
    printf("Verification successful\n");
    return 0;
}