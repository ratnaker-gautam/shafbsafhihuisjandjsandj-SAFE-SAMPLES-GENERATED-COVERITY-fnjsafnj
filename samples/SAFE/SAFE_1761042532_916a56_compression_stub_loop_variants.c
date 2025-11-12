//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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
    return 1;
}

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    output->size = 0;
    if (input_size == 0) return;

    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (output->size + 2 > MAX_INPUT_SIZE) break;
        
        output->data[output->size++] = count;
        output->data[output->size++] = current;
        i = j;
    }
}

void simple_decompress(const uint8_t* input, size_t input_size, Buffer* output) {
    output->size = 0;
    if (input_size == 0 || input_size % 2 != 0) return;

    size_t idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_INPUT_SIZE) break;
        
        size_t k = 0;
        do {
            output->data[output->size++] = value;
            k++;
        } while (k < count);
    }
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    original.size = input_len;
    for (size_t i = 0; i < input_len; i++) {
        original.data[i] = (uint8_t)input_str[i];
    }
    
    printf("\nOriginal data (%zu bytes):\n", original.size);
    print_buffer_hex(&original);
    
    simple_compress(original.data, original.size, &compressed);
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    simple_decompress(compressed.data, compressed.size, &decompressed);
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    printf("\nVerification: ");
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILED\n");
        return 1;
    }
    
    return 0;
}