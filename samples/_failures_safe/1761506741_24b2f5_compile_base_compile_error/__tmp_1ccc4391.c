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

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if (size > max || size < 0) { return EXIT_FAILURE; }

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < len; i += 2) {
        if (i + 1 < len) {
            uint8_t high = (input[i] >> SHIFT_AMOUNT) & 0x0F;
            uint8_t low = (input[i + 1] >> SHIFT_AMOUNT) & 0x0F;
            output[output_index++] = (high << SHIFT_AMOUNT) | low;
        } else {
            output[output_index++] = (input[i] >> SHIFT_AMOUNT) & 0x0F;
        }
    }
    return output_index;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t compressed = input[i];
        uint8_t high = (compressed >> SHIFT_AMOUNT) & 0x0F;
        uint8_t low = compressed & 0x0F;
        
        output[output_index++] = (high << SHIFT_AMOUNT) | (high ^ 0x0F);
        if (output_index < CHUNK_SIZE * 2) {
            output[output_index++] = (low << SHIFT_AMOUNT) | (low ^ 0x0F);
        }
    }
    return output_index;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    VALIDATE_PTR(input);
    VALIDATE_PTR(output);
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_len = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        uint8_t compressed_chunk[CHUNK_SIZE];
        
        int compressed_size = compress_chunk(input + i, chunk_len, compressed_chunk);
        if (compressed_size <= 0) return EXIT_FAILURE;
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
        
        memcpy(output->data + total_compressed, compressed_chunk, compressed_size);
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    CHECK_BOUNDS(input_size, MAX_OUTPUT_SIZE);
    VALIDATE_PTR(input);
    VALIDATE_PTR(output);
    
    output->size = 0;
    size_t total_decompressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t chunk_len = (input_size - i) < (CHUNK_SIZE / COMPRESSION_RATIO) ? 
                          (input_size - i) : (CHUNK_SIZE / COMPRESSION_RATIO);
        uint8_t decompressed_chunk[CHUNK_SIZE * 2];
        
        int decompressed_size = decompress_chunk(input + i, chunk_len, decompressed_chunk);
        if (decompressed_size <= 0) return EXIT_FAILURE;
        
        if (total_decompressed + decompressed_size > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
        
        memcpy(output->data + total_decompressed, decompressed_chunk, decompressed_size);
        total_decompressed += decompressed_size;
    }
    
    output->size = total_decompressed;
    return EXIT_SUCCESS;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    Buffer compressed = {0