//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)
#define RUN_LENGTH_MAX 255
#define BYTE_MASK 0xFF

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_rle_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (count < RUN_LENGTH_MAX && i + count < input_size && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) break;
        
        output->data[output->size++] = count;
        output->data[output->size++] = current;
        i += count;
    }
}

static void process_compression(const uint8_t* input, size_t input_size) {
    Buffer compressed;
    simple_rle_compress(input, input_size, &compressed);
    
    if (compressed.size == 0) {
        printf("Compression failed or produced no output\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 32) printf("...");
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    char line[256];
    
    printf("Enter up to %d bytes of data (hex values, space separated): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned long value;
        char* endptr;
        
        value = strtoul(token, &endptr, 16);
        if (endptr == token || *endptr != '\0') {
            printf("Invalid hex value: %s\n", token);
            return EXIT_FAILURE;
        }
        
        if (value > BYTE_MASK) {
            printf("Value too large for byte: %s\n", token);
            return EXIT_FAILURE;
        }
        
        input_buffer[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input_buffer, input_size)) {
        printf("Invalid input parameters\n");
        return EXIT_FAILURE;
    }
    
    process_compression(input_buffer, input_size);
    
    return EXIT_SUCCESS;
}