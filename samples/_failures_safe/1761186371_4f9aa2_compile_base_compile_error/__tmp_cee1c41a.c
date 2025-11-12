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

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += COMPRESSION_RATIO) {
        if (i + 1 < CHUNK_SIZE) {
            output[compressed_size] = (chunk[i] & 0xF0) | ((chunk[i + 1] >> 4) & 0x0F);
            compressed_size++;
        } else {
            output[compressed_size] = chunk[i] & 0xF0;
            compressed_size++;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* compressed, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / COMPRESSION_RATIO; i++) {
        output[decompressed_size] = (compressed[i] & 0xF0) | MASK_BYTE;
        decompressed_size++;
        if (decompressed_size < CHUNK_SIZE) {
            output[decompressed_size] = ((compressed[i] & 0x0F) << 4) | MASK_BYTE;
            decompressed_size++;
        }
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        memcpy(chunk, current, chunk_size);
        
        if (output->size + (CHUNK_SIZE / COMPRESSION_RATIO) >= MAX_OUTPUT_SIZE) return 0;
        
        size_t compressed_size = compress_chunk(chunk, &output->data[output->size]);
        output->size += compressed_size;
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0) {
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO] = {0};
        size_t chunk_size = (remaining < (CHUNK_SIZE / COMPRESSION_RATIO)) ? remaining : (CHUNK_SIZE / COMPRESSION_RATIO);
        memcpy(compressed_chunk, current, chunk_size);
        
        if (output->size + CHUNK_SIZE >= MAX_OUTPUT_SIZE) return 0;
        
        size_t decompressed_size = decompress_chunk(compressed_chunk, &output->data[output->size]);
        output->size += decompressed_size;
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t data_size = 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &data_size) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid data size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (hex values 00-FF): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "