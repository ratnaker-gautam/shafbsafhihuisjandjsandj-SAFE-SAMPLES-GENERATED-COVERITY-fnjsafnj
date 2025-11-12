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

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size < input_size / COMPRESSION_RATIO + 12) return 0;
    
    compressed_data_t *compressed = (compressed_data_t *)output;
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_size;
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size && compressed_len < output_size - 12; i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        uint8_t sum = 0;
        for (size_t j = 0; j < chunk_size; j++) {
            sum ^= input[i + j];
        }
        if (compressed_len + 1 < output_size - 12) {
            compressed->data[compressed_len++] = sum;
        }
    }
    
    compressed->compressed_size = (uint32_t)compressed_len;
    return compressed_len + 12;
}

static size_t stub_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 12) return 0;
    
    const compressed_data_t *compressed = (const compressed_data_t *)input;
    if (compressed->header != MAGIC_HEADER) return 0;
    if (compressed->original_size > output_size) return 0;
    if (compressed->compressed_size > input_size - 12) return 0;
    
    for (size_t i = 0; i < compressed->original_size; i++) {
        size_t chunk_idx = i / CHUNK_SIZE;
        if (chunk_idx < compressed->compressed_size) {
            output[i] = compressed->data[chunk_idx];
        } else {
            output[i] = 0;
        }
    }
    
    return compressed->original_size;
}

static void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 4 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char buffer[MAX_INPUT_SIZE + 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, buffer, input_len);
    
    size_t compressed_size = stub_compress(input_data, input_len, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f:1\n", (float)input_len / compressed_size);
    
    printf("\nCompressed data (hex):\n");
    print_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = stub_decompress(compressed_data, compressed_size, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");