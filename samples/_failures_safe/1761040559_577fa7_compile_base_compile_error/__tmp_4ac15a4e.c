//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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
    uint8_t data[MAX_OUTPUT_SIZE - 12];
} compressed_data_t;

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    if (output_size < input_size / COMPRESSION_RATIO + 12) return 0;
    
    compressed_data_t *compressed = (compressed_data_t *)output;
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_size;
    
    size_t compressed_bytes = 0;
    for (size_t i = 0; i < input_size && compressed_bytes < sizeof(compressed->data); i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        uint8_t compressed_byte = 0;
        
        for (size_t j = 0; j < chunk_size && j < 8; j++) {
            compressed_byte |= ((input[i + j] & 0x80) >> (7 - j));
        }
        
        if (compressed_bytes < sizeof(compressed->data)) {
            compressed->data[compressed_bytes++] = compressed_byte;
        }
    }
    
    compressed->compressed_size = (uint32_t)compressed_bytes;
    return compressed_bytes + 12;
}

static size_t stub_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 12) return 0;
    
    const compressed_data_t *compressed = (const compressed_data_t *)input;
    if (compressed->header != MAGIC_HEADER) return 0;
    if (compressed->original_size > output_size) return 0;
    if (compressed->compressed_size > sizeof(compressed->data)) return 0;
    
    size_t decompressed_bytes = 0;
    for (size_t i = 0; i < compressed->compressed_size && decompressed_bytes < compressed->original_size; i++) {
        uint8_t compressed_byte = compressed->data[i];
        
        for (int bit = 7; bit >= 0 && decompressed_bytes < compressed->original_size; bit--) {
            output[decompressed_bytes++] = (compressed_byte & (1 << bit)) ? 0xFF : 0x00;
        }
    }
    
    return decompressed_bytes;
}

static void print_hex(const uint8_t *data, size_t size) {
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
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter test data (up to %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[128];
    size_t input_size = 0;
    
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        char *pos = line;
        while (*pos != '\0' && input_size < MAX_INPUT_SIZE) {
            if (*pos == ' ' || *pos == '\t' || *pos == '\n') {
                pos++;
                continue;
            }
            
            if (pos[0] == '\0' || pos[1] == '\0') break;
            
            unsigned int byte;
            if (sscanf(pos, "%02X", &byte) == 1) {
                input_data[input_size++] = (uint8_t)byte;
                pos += 2;
            } else {
                break;
            }
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    size_t compressed_size = stub_compress(input_data, input_size, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        printf("