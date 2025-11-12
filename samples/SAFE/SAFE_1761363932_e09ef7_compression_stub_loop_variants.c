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

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void simple_compress(const char* input, size_t input_len, Buffer* output) {
    if (input_len == 0) return;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = (uint8_t)current;
        
        i += count;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, char* output, size_t* output_len) {
    if (input->length == 0 || input->length % 2 != 0) {
        *output_len = 0;
        return;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->length; i += 2) {
        uint8_t count = input->data[i];
        char character = (char)input->data[i + 1];
        
        if (out_idx + count >= MAX_OUTPUT_SIZE) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = character;
        }
    }
    
    *output_len = out_idx;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    Buffer compressed;
    char decompressed[MAX_INPUT_SIZE];
    size_t decompressed_len = 0;
    
    initialize_buffer(&compressed);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
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
    
    simple_compress(input, input_len, &compressed);
    
    printf("Compressed size: %zu bytes (original: %zu bytes)\n", 
           compressed.length, input_len);
    
    if (compressed.length > 0) {
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed.length; i++) {
            printf("%02X ", compressed.data[i]);
        }
        printf("\n");
    }
    
    simple_decompress(&compressed, decompressed, &decompressed_len);
    
    if (decompressed_len > 0) {
        decompressed[decompressed_len] = '\0';
        printf("Decompressed text: %s\n", decompressed);
        
        if (strncmp(input, decompressed, input_len) == 0 && 
            decompressed_len == input_len) {
            printf("Verification: SUCCESS\n");
        } else {
            printf("Verification: FAILED\n");
            return 1;
        }
    } else {
        printf("Decompression failed\n");
        return 1;
    }
    
    return 0;
}