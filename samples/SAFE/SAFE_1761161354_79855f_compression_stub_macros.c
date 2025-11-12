//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_data(const uint8_t* input, size_t input_size, compressed_data_t* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->magic = MAGIC_NUMBER;
    output->original_size = (uint32_t)input_size;
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size && output_index < MAX_OUTPUT_SIZE - 1; i += COMPRESSION_RATIO) {
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        output->data[output_index++] = compressed_byte;
    }
    
    return output_index;
}

static size_t decompress_data(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input->magic != MAGIC_NUMBER) return 0;
    if (input->original_size > MAX_INPUT_SIZE) return 0;
    if (output_size < input->original_size) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < MAX_OUTPUT_SIZE && output_index < input->original_size; i++) {
        uint8_t compressed_byte = input->data[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && output_index < input->original_size; j--) {
            output[output_index++] = (compressed_byte >> (j * 2)) & 0x03;
        }
    }
    
    return output_index;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size && i < MAX_OUTPUT_SIZE; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 4 == 0) printf(" ");
    }
    if (size > 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    size_t input_size = 0;
    
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        char* pos = line;
        while (*pos != '\0' && input_size < MAX_INPUT_SIZE) {
            if (*pos == ' ' || *pos == '\n') {
                pos++;
                continue;
            }
            
            if (sscanf(pos, "%2hhx", &input_data[input_size]) == 1) {
                input_size++;
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
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    size_t compressed_size = compress_data(input_data, input_size, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    size_t decompressed_size = decompress_data(&compressed, decompressed, sizeof(decompressed));
    if (decompressed_size != input_size) {
        printf("Decompression failed. Expected %zu bytes, got %zu bytes.\n", input_size, decompressed_size);
        return 1;
    }
    
    if (memcmp(input_data, decompressed, input_size) != 0) {
        printf("Decompression verification failed.\n");
        return 1;
    }
    
    printf("Decompression successful. Data verified.\n");
    printf("Compression ratio: %.2f:1\n", (float)input_size / compressed_size);
    
    return 0;
}