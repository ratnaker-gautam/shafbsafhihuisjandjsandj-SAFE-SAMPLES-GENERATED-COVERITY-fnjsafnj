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
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == '\\' || current == '\'') {
            output[out_idx++] = '\\';
            if (out_idx >= output_max) return 0;
            output[out_idx++] = (uint8_t)count;
            if (out_idx >= output_max) return 0;
            output[out_idx++] = current;
            if (out_idx >= output_max) return 0;
        } else {
            for (size_t j = 0; j < count && out_idx < output_max; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max) {
        if (input[i] == '\\' && i + 2 < input_len) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (count == 0 || out_idx + count > output_max) return 0;
            
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
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) return 0;
    
    size_t compressed_size = simple_compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) return 0;
    
    output->size = compressed_size;
    return 1;
}

int decompress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t decompressed_size = simple_decompress(input->data, input->size, output->data, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_INPUT_SIZE) return 0;
    
    output->size = decompressed_size;
    return 1;
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
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char input_line[MAX_INPUT_SIZE + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input_line, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n