//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCDEF00

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    if (output_size < input_size * COMPRESSION_RATIO) return 0;
    
    size_t output_pos = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        if (output_pos + chunk_size + 1 > output_size) return 0;
        
        output[output_pos++] = (uint8_t)chunk_size;
        for (size_t j = 0; j < chunk_size; j++) {
            output[output_pos++] = input[i + j] ^ 0x55;
        }
    }
    return output_pos;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    size_t output_pos = 0;
    size_t input_pos = 0;
    
    while (input_pos < input_size) {
        if (input_pos >= input_size) return 0;
        uint8_t chunk_size = input[input_pos++];
        
        if (chunk_size == 0 || chunk_size > CHUNK_SIZE) return 0;
        if (input_pos + chunk_size > input_size) return 0;
        if (output_pos + chunk_size > output_size) return 0;
        
        for (uint8_t j = 0; j < chunk_size; j++) {
            output[output_pos++] = input[input_pos + j] ^ 0x55;
        }
        input_pos += chunk_size;
    }
    return output_pos;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 4 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_buffer, input_size);
    
    size_t compressed_size = stub_compress(input_buffer, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(output_buffer, compressed_size);
    
    size_t decompressed_size = stub_decompress(output_buffer, compressed_size, decompressed_buffer, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: %s\n", decompressed_buffer);
    
    return EXIT_SUCCESS;
}