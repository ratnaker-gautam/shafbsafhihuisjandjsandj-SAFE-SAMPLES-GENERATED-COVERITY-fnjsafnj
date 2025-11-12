//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[output_index++] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
        } else {
            output[output_index++] = (chunk[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t current_chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (total_compressed + COMPRESSION_RATIO > MAX_OUTPUT_SIZE) return 0;
        
        size_t compressed_size = compress_chunk(input + i, current_chunk_size, 
                                              output->data + total_compressed);
        if (compressed_size == 0) return 0;
        
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed_data;
    
    printf("Enter data to compress (up to %d bytes as hex, space separated):\n", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return EXIT_FAILURE;
        }
        
        char* endptr;
        unsigned long value = strtoul(token, &endptr, 16);
        if (*endptr != '\0' || value > UINT8_MAX) {
            fprintf(stderr, "Invalid hex value: %s\n", token);
            return EXIT_FAILURE;
        }
        
        input_data[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return EXIT_FAILURE;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    if (!compress_data(input_data, input_size, &compressed_data)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_data.size);
    print_hex(compressed_data.data, compressed_data.size);
    
    printf("\nCompression ratio: %.2f:1\n", 
           (float)input_size / (float)compressed_data.size);
    
    return EXIT_SUCCESS;
}