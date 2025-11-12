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

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    if (output_size < input_size * COMPRESSION_RATIO) return 0;
    
    compressed_data_t* comp = (compressed_data_t*)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint16_t)input_size;
    
    size_t compressed_bytes = 0;
    for (size_t i = 0; i < input_size && compressed_bytes < output_size - sizeof(compressed_data_t); i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        if (compressed_bytes + chunk_size > output_size - sizeof(compressed_data_t)) break;
        
        memcpy(&comp->data[compressed_bytes], &input[i], chunk_size);
        compressed_bytes += chunk_size;
    }
    
    comp->compressed_size = (uint16_t)compressed_bytes;
    return sizeof(compressed_data_t) + compressed_bytes;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < sizeof(compressed_data_t)) return 0;
    
    const compressed_data_t* comp = (const compressed_data_t*)input;
    if (comp->header != MAGIC_HEADER) return 0;
    if (comp->original_size > output_size) return 0;
    if (comp->compressed_size > input_size - sizeof(compressed_data_t)) return 0;
    
    size_t decompressed_bytes = 0;
    for (size_t i = 0; i < comp->compressed_size && decompressed_bytes < output_size; i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= comp->compressed_size) ? CHUNK_SIZE : comp->compressed_size - i;
        if (decompressed_bytes + chunk_size > output_size) break;
        
        memcpy(&output[decompressed_bytes], &comp->data[i], chunk_size);
        decompressed_bytes += chunk_size;
    }
    
    if (decompressed_bytes != comp->original_size) return 0;
    return decompressed_bytes;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (!validate_input(input_buffer, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = stub_compress(input_buffer, input_size, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = stub_decompress(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed successfully: %s\n", decompressed_buffer);
    return EXIT_SUCCESS;
}