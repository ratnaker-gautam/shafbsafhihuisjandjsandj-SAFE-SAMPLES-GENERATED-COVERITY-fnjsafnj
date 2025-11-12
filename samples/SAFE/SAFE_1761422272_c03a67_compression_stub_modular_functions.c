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

int compress_run_length(const char* input, size_t input_len, Buffer* output) {
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output->size >= MAX_OUTPUT_SIZE - 2) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        
        i += count;
    }
    
    return 1;
}

int decompress_run_length(const Buffer* input, char* output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t character = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = character;
        }
    }
    
    *output_len = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_len;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original text: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_run_length(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!decompress_run_length(&compressed, output, &decompressed_len)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    output[decompressed_len] = '\0';
    
    if (decompressed_len != input_len || memcmp(input, output, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Decompressed text: %s\n", output);
    printf("Decompression verified successfully\n");
    
    if (compressed.size < input_len) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    } else {
        printf("No compression achieved\n");
    }
    
    return 0;
}