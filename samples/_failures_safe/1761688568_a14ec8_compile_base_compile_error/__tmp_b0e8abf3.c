//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size && output->size < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count && output->size < MAX_OUTPUT_SIZE; j++) {
            output->data[output->size++] = value;
        }
    }
}

int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter input data (up to %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < max_size - 1 && (c = getchar()) != EOF && c != '\n') {
        buffer[total_read++] = (uint8_t)c;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

void print_buffer_ascii(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        if (buffer->data[i] >= 32 && buffer->data[i] <= 126) {
            printf("%c", buffer->data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    
    if (input_size <= 0) {
        printf("No input data received.\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    printf("Hex: ");
    for (int i = 0; i < input_size; i++) {
        printf("%02X ", input[i]);
    }
    printf("\nASCII: ");
    for (int i = 0; i < input_size; i++) {
        if (input[i] >= 32 && input[i] <= 126) {
            printf("%c", input[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
    
    compress_rle(input, (size_t)input_size, &compressed);
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    printf("Hex: ");
    print_buffer_hex(&compressed);
    printf("Format: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i + 1 < compressed.size) {
            printf("[%d x 0x%02X] ", compressed.data[i], compressed.data[i + 1]);
        }
    }
    printf("\n");
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    printf("Hex: ");
    print_buffer_hex(&decompressed);
    printf("ASCII: ");
    print_buffer_ascii(&decompressed);
    
    if (decompressed.size == (size_t)input_size && 
        memcmp(input, decompressed.data, decompressed.size) == 0) {
        printf("\nCompression/Decompression successful!\n");
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / (double)input_size) * 100.0);
    } else {
        printf("\nCompression/Decompression failed!\n");
        return 1;
    }
    
    return