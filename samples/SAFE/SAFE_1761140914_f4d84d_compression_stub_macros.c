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
#define MAGIC_HEADER 0xABCD1234

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE - 12];
} compressed_data_t;

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size < input_size / COMPRESSION_RATIO + 12) return 0;
    
    compressed_data_t* comp = (compressed_data_t*)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint32_t)input_size;
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size && compressed_len < output_size - 12; i += CHUNK_SIZE) {
        size_t chunk_len = (input_size - i < CHUNK_SIZE) ? input_size - i : CHUNK_SIZE;
        uint8_t sum = 0;
        for (size_t j = 0; j < chunk_len; j++) {
            sum ^= input[i + j];
        }
        if (compressed_len + 1 < output_size - 12) {
            output[12 + compressed_len] = sum;
            compressed_len++;
        }
    }
    
    comp->compressed_size = (uint32_t)compressed_len;
    return compressed_len + 12;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 12) return 0;
    
    const compressed_data_t* comp = (const compressed_data_t*)input;
    if (comp->header != MAGIC_HEADER) return 0;
    if (comp->original_size == 0 || comp->original_size > MAX_INPUT_SIZE) return 0;
    if (comp->compressed_size > input_size - 12) return 0;
    if (output_size < comp->original_size) return 0;
    
    for (size_t i = 0; i < comp->original_size; i++) {
        size_t chunk_idx = i / CHUNK_SIZE;
        if (chunk_idx < comp->compressed_size) {
            output[i] = input[12 + chunk_idx];
        } else {
            output[i] = 0;
        }
    }
    
    return comp->original_size;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size && i < 32; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_data, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    print_hex(input_data, input_len);
    
    size_t compressed_size = stub_compress(input_data, input_len, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    print_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = stub_decompress(compressed_data, compressed_size, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_hex(decompressed_data, decompressed_size);
    
    if (decompressed_size == input_len && memcmp(input_data, decompressed_data, input_len) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    return 0;
}