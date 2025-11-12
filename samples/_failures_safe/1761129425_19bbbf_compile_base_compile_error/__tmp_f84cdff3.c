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
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len < output_max - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\') {
            if (output_len + 3 > output_max) break;
            output[output_len++] = '\\';
            output[output_len++] = (uint8_t)count;
            output[output_len++] = current;
        } else {
            for (size_t j = 0; j < count && output_len < output_max; j++) {
                output[output_len++] = current;
            }
        }
        
        i += count;
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len < output_max) {
        if (input[i] == '\\' && i + 2 < input_len) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_len + count > output_max) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_len++] = value;
            }
            
            i += 3;
        } else {
            output[output_len++] = input[i++];
        }
    }
    
    return output_len;
}

int process_compression(const char* input_str, Buffer* compressed) {
    if (input_str == NULL || compressed == NULL) return 0;
    
    size_t input_len = strlen(input_str);
    if (!validate_input(input_str, input_len)) return 0;
    
    compressed->size = simple_compress((const uint8_t*)input_str, input_len, 
                                     compressed->data, MAX_OUTPUT_SIZE);
    
    return compressed->size > 0;
}

int process_decompression(const Buffer* compressed, char* output, size_t output_size) {
    if (compressed == NULL || output == NULL || output_size == 0) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t decompressed_len = simple_decompress(compressed->data, compressed->size,
                                              (uint8_t*)output, output_size - 1);
    
    if (decompressed_len == 0) return 0;
    
    output[decompressed_len] = '\0';
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL || buffer->size == 0) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02x", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buffer->size) printf(" ");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!process_compression(input, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!process_decompression(&compressed, output, sizeof(output)))