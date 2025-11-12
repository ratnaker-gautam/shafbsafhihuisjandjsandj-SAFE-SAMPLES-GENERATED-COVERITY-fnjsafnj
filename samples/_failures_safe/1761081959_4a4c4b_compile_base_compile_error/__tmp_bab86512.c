//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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
            uint8_t compressed = (chunk[i] & 0xF0) | ((chunk[i + 1] >> 4) & 0x0F);
            output[compressed_size++] = compressed ^ MASK_BYTE;
        } else {
            output[compressed_size++] = chunk[i] ^ MASK_BYTE;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / COMPRESSION_RATIO; i++) {
        uint8_t processed = chunk[i] ^ MASK_BYTE;
        output[decompressed_size++] = (processed & 0xF0) | 0x0F;
        if (decompressed_size < CHUNK_SIZE) {
            output[decompressed_size++] = ((processed & 0x0F) << 4) | 0x0F;
        }
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    
    while (remaining > 0 && output->size < MAX_OUTPUT_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        memcpy(chunk, input + (input_size - remaining), chunk_size);
        
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) break;
        
        memcpy(output->data + output->size, compressed_chunk, compressed_size);
        output->size += compressed_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    
    while (remaining > 0 && output->size < MAX_OUTPUT_SIZE) {
        size_t chunk_size = (remaining < (CHUNK_SIZE / COMPRESSION_RATIO)) ? 
                           remaining : (CHUNK_SIZE / COMPRESSION_RATIO);
        
        uint8_t decompressed_chunk[CHUNK_SIZE];
        size_t decompressed_size = decompress_chunk(input + (input_size - remaining), 
                                                   decompressed_chunk);
        
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) break;
        
        memcpy(output->data + output->size, decompressed_chunk, decompressed_size);
        output->size += decompressed_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    printf("Enter %d bytes as hex values (e.g., A1 B2 C3): ", size_input);
    for (int i = 0; i < size_input; i++) {
        unsigned int byte;
        if (scanf("%02x