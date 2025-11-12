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

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->length == 0) {
        output->length = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
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
        i = j;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input->length == 0 || input->length % 2 != 0) {
        output->length = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    for (i = 0; i < input->length; i += 2) {
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
    }
    
    output->length = out_idx;
}

int buffers_equal(const Buffer* a, const Buffer* b) {
    if (a->length != b->length) {
        return 0;
    }
    
    for (size_t idx = 0; idx < a->length; idx++) {
        if (a->data[idx] != b->data[idx]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (!validate_input((uint8_t*)input_line, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(original.data, input_line, input_len);
    original.length = input_len;
    
    simple_compress(&original, &compressed);
    
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    if (compressed.length < original.length) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.length / original.length) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
    
    simple_decompress(&compressed, &decompressed);
    
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (!buffers_equal(&original, &decompressed)) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    printf("Compressed data (hex): ");
    for (size_t pos = 0; pos < compressed.length; pos++) {
        printf("%02X ", compressed.data[pos]);
    }
    printf("\n");
    
    return 0;
}