//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const Buffer* compressed, Buffer* output) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0) return 0;
    if (compressed->size % 2 != 0) return 0;
    
    output->size = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
    
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if (i < buffer->size - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    uint8_t test_input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    const char* test_string = "AAAABBBCCDAA";
    size_t test_len = strlen(test_string);
    
    if (test_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Test input too large\n");
        return 1;
    }
    
    memcpy(test_input, test_string, test_len);
    
    printf("Original: %s\n", test_string);
    printf("Original (hex): ");
    for (size_t i = 0; i < test_len; i++) {
        printf("%02X ", test_input[i]);
    }
    printf("\n");
    
    if (!compress_rle(test_input, test_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    printf("Compression ratio: %.2f\n", (float)test_len / compressed.size);
    
    if (!decompress_rle(&compressed, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != test_len) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(test_input, decompressed.data, test_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Verification: PASS\n");
    return 0;
}