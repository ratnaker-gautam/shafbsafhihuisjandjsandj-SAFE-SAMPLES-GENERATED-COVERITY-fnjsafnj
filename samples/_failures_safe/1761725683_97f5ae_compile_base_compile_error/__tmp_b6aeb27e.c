//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max - 3) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && out_idx < output_max; j++) {
                output[out_idx++] = current;
            }
            i += count;
        }
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max) {
        if (i + 2 < input_len && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > output_max) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            i += 3;
        } else {
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

int compress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size > MAX_INPUT_SIZE) return 0;
    
    output->size = simple_compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    return output->size > 0;
}

int decompress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size > MAX_OUTPUT_SIZE) return 0;
    
    output->size = simple_decompress(input->data, input->size, output->data, MAX_INPUT_SIZE);
    return output->size > 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buf->size) printf(" ");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main(void) {
    char user_input[MAX_INPUT_SIZE + 1];
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    original.size = input_len;
    memcpy(original.data, user_input, input_len);
    
    printf("\nOriginal data (%zu bytes):\n", original.size);
    print_buffer_hex(&original);
    
    if (!compress_buffer(&original, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!decompress_buffer(&compressed, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decomp