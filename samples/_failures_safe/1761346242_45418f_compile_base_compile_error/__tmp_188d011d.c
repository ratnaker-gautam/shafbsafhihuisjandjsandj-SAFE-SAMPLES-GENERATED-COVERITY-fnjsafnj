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
#define MASK_BYTE 0xAA

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

static int compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size > CHUNK_SIZE) return 0;
    if (output == NULL) return 0;
    
    uint8_t compressed = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t masked = chunk[i] ^ MASK_BYTE;
        compressed |= (masked & 0x0F) << (4 * i);
    }
    *output = compressed;
    return 1;
}

static int decompress_chunk(uint8_t compressed, uint8_t* output, size_t output_size) {
    if (output == NULL) return 0;
    if (output_size < CHUNK_SIZE) return 0;
    
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        uint8_t value = (compressed >> (4 * i)) & 0x0F;
        output[i] = value ^ (MASK_BYTE & 0x0F);
    }
    return 1;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t num_chunks = (input_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    
    if (num_chunks > MAX_OUTPUT_SIZE) return 0;
    
    for (size_t i = 0; i < num_chunks; i++) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = CHUNK_SIZE;
        if (i * CHUNK_SIZE + CHUNK_SIZE > input_size) {
            chunk_size = input_size - i * CHUNK_SIZE;
        }
        
        memcpy(chunk, input + i * CHUNK_SIZE, chunk_size);
        
        uint8_t compressed;
        if (!compress_chunk(chunk, chunk_size, &compressed)) {
            return 0;
        }
        
        if (output->size >= MAX_OUTPUT_SIZE) return 0;
        output->data[output->size++] = compressed;
    }
    
    return 1;
}

static int decompress_data(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL) return 0;
    if (output == NULL) return 0;
    if (input->size == 0) return 0;
    if (input->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t expected_size = input->size * CHUNK_SIZE;
    if (output_size < expected_size) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        uint8_t chunk[CHUNK_SIZE];
        if (!decompress_chunk(input->data[i], chunk, CHUNK_SIZE)) {
            return 0;
        }
        memcpy(output + i * CHUNK_SIZE, chunk, CHUNK_SIZE);
    }
    
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t data_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &data_size) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes as hex values: ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            fprintf(stderr, "Invalid hex input\n");
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            fprintf(stderr, "Value out of range\n");
            return EXIT_FAILURE;
        }
        original_data[i] = (uint8_t)value;
    }
    
    Buffer compressed;
    if (!compress_data(original_data, data_size, &compressed)) {
        fprintf(stderr, "Compression