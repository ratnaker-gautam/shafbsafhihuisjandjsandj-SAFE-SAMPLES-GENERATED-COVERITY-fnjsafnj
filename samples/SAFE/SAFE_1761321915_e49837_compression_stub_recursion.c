//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void compress_recursive(const uint8_t* input, size_t len, size_t pos, Buffer* output) {
    if (pos >= len) {
        return;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (output->length + 2 <= MAX_INPUT_SIZE) {
        output->data[output->length++] = (uint8_t)count;
        output->data[output->length++] = current;
    }
    
    compress_recursive(input, len, pos + count, output);
}

int decompress_recursive(const uint8_t* input, size_t len, size_t pos, Buffer* output) {
    if (pos >= len) {
        return 1;
    }
    
    if (pos + 1 >= len) {
        return 0;
    }
    
    uint8_t count = input[pos];
    uint8_t value = input[pos + 1];
    
    if (count == 0 || output->length + count > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < count; i++) {
        output->data[output->length++] = value;
    }
    
    return decompress_recursive(input, len, pos + 2, output);
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->length - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
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
    
    memcpy(original.data, input_str, input_len);
    original.length = input_len;
    
    printf("Original: %s\n", input_str);
    printf("Original (hex): ");
    print_buffer_hex(&original);
    
    compress_recursive(original.data, original.length, 0, &compressed);
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    
    if (!decompress_recursive(compressed.data, compressed.length, 0, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.length; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    if (original.length != decompressed.length || 
        memcmp(original.data, decompressed.data, original.length) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.length / original.length * 100);
    
    return 0;
}