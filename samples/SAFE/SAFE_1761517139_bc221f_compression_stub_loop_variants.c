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

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void compress_run_length(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->length = out_idx;
}

void decompress_run_length(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        if (i + 1 >= input->length) {
            output->length = 0;
            return;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
        
        i += 2;
    }
    
    output->length = out_idx;
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(original.data, input_line, input_len);
    original.length = input_len;
    
    if (!validate_input(original.data, original.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    compress_run_length(&original, &compressed);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    decompress_run_length(&compressed, &decompressed);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Original length: %zu\n", original.length);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Compression ratio: %.2f\n", (float)compressed.length / original.length);
    
    printf("Decompressed: ");
    for (size_t idx = 0; idx < decompressed.length; idx++) {
        putchar(decompressed.data[idx]);
    }
    putchar('\n');
    
    if (original.length == decompressed.length && 
        memcmp(original.data, decompressed.data, original.length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}