//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    output->size = 0;
    if (input->size == 0) {
        return;
    }
    
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->size && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (output->size + 2 > MAX_INPUT_SIZE) {
            break;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    output->size = 0;
    if (input->size == 0 || input->size % 2 != 0) {
        return;
    }
    
    for (size_t idx = 0; idx < input->size; idx += 2) {
        uint8_t count = input->data[idx];
        uint8_t value = input->data[idx + 1];
        
        if (output->size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[output->size++] = value;
            k++;
        } while (k < count);
    }
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    char user_input[MAX_INPUT_SIZE + 1];
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[--input_len] = '\0';
    }
    
    if (!validate_input(user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    original.size = input_len;
    for (size_t i = 0; i < input_len; i++) {
        original.data[i] = (uint8_t)user_input[i];
    }
    
    simple_compress(&original, &compressed);
    simple_decompress(&compressed, &decompressed);
    
    printf("Original size: %zu\n", original.size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (original.size > 0) ? (100.0 * compressed.size / original.size) : 0.0);
    
    printf("Original: %s\n", user_input);
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    
    if (decompressed.size == original.size && 
        memcmp(decompressed.data, original.data, original.size) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    return 0;
}