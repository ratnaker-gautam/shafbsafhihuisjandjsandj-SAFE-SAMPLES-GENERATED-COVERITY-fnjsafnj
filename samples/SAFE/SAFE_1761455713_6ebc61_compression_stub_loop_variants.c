//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size == 0) {
        output->size = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i = j;
    }
    
    output->size = out_idx;
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size == 0 || input_size % 2 != 0) {
        output->size = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    for (i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        size_t j = 0;
        do {
            output->data[out_idx++] = value;
            j++;
        } while (j < count);
    }
    
    output->size = out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)temp_input[i];
    }
    
    compress_run_length(input_data, input_len, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == 0 || decompressed.size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    int match = 1;
    for (size_t i = 0; i < input_len; i++) {
        if (input_data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed.size / input_len * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}