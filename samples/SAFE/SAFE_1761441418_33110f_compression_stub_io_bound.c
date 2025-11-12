//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

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

int compress_rle(const char* input, size_t input_len, Buffer* output) {
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const Buffer* input, char* output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_len = out_idx;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    Buffer compressed;
    
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
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_rle(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / input_len * 100.0f);
    }
    
    size_t decompressed_len;
    if (!decompress_rle(&compressed, output, &decompressed_len)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    output[decompressed_len] = '\0';
    
    if (decompressed_len != input_len || memcmp(input, output, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", output);
    printf("Verification successful\n");
    
    return 0;
}