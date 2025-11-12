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
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE - 8];
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
    if (output_size < input_size / COMPRESSION_RATIO + 8) return 0;
    
    compressed_data_t* comp = (compressed_data_t*)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint16_t)input_size;
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size && compressed_len < output_size - 8; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        if (compressed_len + chunk_len / COMPRESSION_RATIO + 1 > output_size - 8) break;
        
        output[8 + compressed_len] = (uint8_t)chunk_len;
        compressed_len++;
        
        for (size_t j = 0; j < chunk_len; j += COMPRESSION_RATIO) {
            if (compressed_len >= output_size - 8) break;
            uint8_t compressed_byte = 0;
            for (size_t k = 0; k < COMPRESSION_RATIO && j + k < chunk_len; k++) {
                compressed_byte |= (input[i + j + k] & 0xC0) >> (2 * (COMPRESSION_RATIO - 1 - k));
            }
            output[8 + compressed_len] = compressed_byte;
            compressed_len++;
        }
    }
    
    comp->compressed_size = (uint16_t)compressed_len;
    return compressed_len + 8;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 8) return 0;
    
    const compressed_data_t* comp = (const compressed_data_t*)input;
    if (comp->header != MAGIC_HEADER) return 0;
    if (comp->original_size > output_size) return 0;
    if (comp->compressed_size > input_size - 8) return 0;
    
    size_t decompressed_len = 0;
    size_t comp_pos = 0;
    
    while (comp_pos < comp->compressed_size && decompressed_len < comp->original_size) {
        if (comp_pos >= comp->compressed_size) break;
        uint8_t chunk_len = input[8 + comp_pos];
        comp_pos++;
        
        if (chunk_len == 0 || decompressed_len + chunk_len > comp->original_size) break;
        
        for (size_t i = 0; i < chunk_len && comp_pos < comp->compressed_size; i += COMPRESSION_RATIO) {
            if (comp_pos >= comp->compressed_size) break;
            uint8_t compressed_byte = input[8 + comp_pos];
            comp_pos++;
            
            for (size_t j = 0; j < COMPRESSION_RATIO && i + j < chunk_len; j++) {
                uint8_t shift = 2 * (COMPRESSION_RATIO - 1 - j);
                output[decompressed_len + i + j] = (compressed_byte & (0x03 << shift)) << (6 - shift);
            }
        }
        decompressed_len += chunk_len;
    }
    
    return decompressed_len;
}

static int compare_data(const uint8_t* a, const uint8_t* b, size_t size) {
    if (a == NULL || b == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    fflush(stdout);
    
    if (fgets((char*)original, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t original_len = strlen((char*)original);
    if (original_len == 0) {
        fprintf(stderr, "Empty input\n");
        return