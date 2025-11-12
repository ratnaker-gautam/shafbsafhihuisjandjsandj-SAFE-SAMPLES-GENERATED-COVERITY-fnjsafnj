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
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (b != 0 && a > SIZE_MAX / b) return SIZE_MAX;
    return a * b;
}

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    size_t output_size = 0;
    for (size_t i = 0; i < input_size && i < CHUNK_SIZE; i += 2) {
        if (i + 1 < input_size) {
            uint8_t high = input[i] >> 4;
            uint8_t low = input[i + 1] >> 4;
            output[output_size++] = (high << 4) | low;
        } else {
            output[output_size++] = input[i] >> 4;
        }
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        output[output_size++] = (input[i] >> 4) << 4;
        if (output_size < CHUNK_SIZE * 2) {
            output[output_size++] = (input[i] & 0x0F) << 4;
        }
    }
    return output_size;
}

static compressed_data_t* compress_data(const uint8_t* input, size_t input_size) {
    if (!validate_input(input, input_size)) return NULL;
    
    size_t max_output_size = safe_multiply(input_size, COMPRESSION_RATIO);
    if (max_output_size > MAX_OUTPUT_SIZE) return NULL;
    
    size_t total_size = safe_add(sizeof(compressed_data_t), max_output_size);
    if (total_size > MAX_OUTPUT_SIZE) return NULL;
    
    compressed_data_t* result = malloc(total_size);
    if (result == NULL) return NULL;
    
    result->header = MAGIC_HEADER;
    result->original_size = (uint16_t)input_size;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t chunk_compressed = compress_chunk(input + i, chunk_size, result->data + compressed_size);
        compressed_size += chunk_compressed;
        if (compressed_size > max_output_size) {
            free(result);
            return NULL;
        }
    }
    
    result->compressed_size = (uint16_t)compressed_size;
    return result;
}

static uint8_t* decompress_data(const compressed_data_t* compressed) {
    if (compressed == NULL) return NULL;
    if (compressed->header != MAGIC_HEADER) return NULL;
    if (compressed->original_size == 0 || compressed->original_size > MAX_INPUT_SIZE) return NULL;
    if (compressed->compressed_size == 0 || compressed->compressed_size > MAX_OUTPUT_SIZE) return NULL;
    
    uint8_t* result = malloc(compressed->original_size);
    if (result == NULL) return NULL;
    
    size_t decompressed_size = 0;
    for (size_t i = 0; i < compressed->compressed_size; ) {
        size_t remaining_input = compressed->compressed_size - i;
        size_t chunk_input_size = remaining_input < CHUNK_SIZE ? remaining_input : CHUNK_SIZE;
        size_t chunk_decompressed = decompress_chunk(compressed->data + i, chunk_input_size, result + decompressed_size);
        decompressed_size += chunk_decompressed;
        i += chunk_input_size;
        
        if (decompressed_size > compressed->original_size) {
            free(result);
            return NULL;
        }
    }
    
    if (decompressed_size != compressed->original_size) {
        free(result);
        return NULL;
    }
    
    return result;
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

int