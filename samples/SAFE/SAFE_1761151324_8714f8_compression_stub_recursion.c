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
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

void compress_run_length_recursive(const uint8_t* input, size_t len, size_t pos, Buffer* output) {
    if (pos >= len) {
        return;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (output->length + 2 <= MAX_INPUT_SIZE) {
        output->data[output->length++] = current;
        output->data[output->length++] = (uint8_t)count;
    }
    
    compress_run_length_recursive(input, len, pos + count, output);
}

void decompress_run_length_recursive(const uint8_t* input, size_t len, size_t pos, Buffer* output) {
    if (pos >= len || pos + 1 >= len) {
        return;
    }
    
    uint8_t character = input[pos];
    uint8_t count = input[pos + 1];
    
    if (output->length + count > MAX_INPUT_SIZE) {
        count = MAX_INPUT_SIZE - output->length;
    }
    
    for (uint8_t i = 0; i < count && output->length < MAX_INPUT_SIZE; i++) {
        output->data[output->length++] = character;
    }
    
    decompress_run_length_recursive(input, len, pos + 2, output);
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

void print_buffer_text(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%c", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
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
    
    printf("Original: %s\n", input_str);
    printf("Original length: %zu\n", input_len);
    
    compress_run_length_recursive((uint8_t*)input_str, input_len, 0, &compressed);
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    printf("Compressed length: %zu\n", compressed.length);
    
    decompress_run_length_recursive(compressed.data, compressed.length, 0, &decompressed);
    printf("Decompressed: ");
    print_buffer_text(&decompressed);
    
    if (input_len == decompressed.length && 
        memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}