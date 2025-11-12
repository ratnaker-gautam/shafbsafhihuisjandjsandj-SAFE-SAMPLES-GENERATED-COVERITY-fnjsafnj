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

void simple_decompress(const Buffer* input, Buffer* output) {
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
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
        
        i += 2;
    }
    
    output->length = out_idx;
}

int buffers_equal(const Buffer* a, const Buffer* b) {
    if (a->length != b->length) {
        return 0;
    }
    
    for (size_t i = 0; i < a->length; i++) {
        if (a->data[i] != b->data[i]) {
            return 0;
        }
    }
    
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char input_str[MAX_INPUT_SIZE + 1];
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input((uint8_t*)input_str, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(original.data, input_str, len);
    original.length = len;
    
    printf("Original data (%zu bytes): ", original.length);
    print_buffer_hex(&original);
    
    simple_compress(&original, &compressed);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.length);
    print_buffer_hex(&compressed);
    
    simple_decompress(&compressed, &decompressed);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.length);
    print_buffer_hex(&decompressed);
    
    if (buffers_equal(&original, &decompressed)) {
        printf("Compression/decompression successful!\n");
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (float)compressed.length / original.length) * 100.0);
    } else {
        printf("Compression/decompression failed - data mismatch!\n");
        return 1;
    }
    
    return 0;
}