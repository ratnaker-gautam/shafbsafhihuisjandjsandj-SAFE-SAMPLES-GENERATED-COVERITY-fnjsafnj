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
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE - 12];
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
    if (output_size < input_size / COMPRESSION_RATIO + 12) return 0;
    
    compressed_data_t* comp = (compressed_data_t*)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint32_t)input_size;
    
    size_t compressed_bytes = 0;
    for (size_t i = 0; i < input_size && compressed_bytes < sizeof(comp->data); i += CHUNK_SIZE) {
        size_t chunk_end = (i + CHUNK_SIZE < input_size) ? i + CHUNK_SIZE : input_size;
        size_t chunk_size = chunk_end - i;
        
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < chunk_size && j < 8; j++) {
            compressed_byte |= ((input[i + j] & 1) << j);
        }
        
        if (compressed_bytes < sizeof(comp->data)) {
            comp->data[compressed_bytes] = compressed_byte;
            compressed_bytes++;
        }
    }
    
    comp->compressed_size = (uint32_t)compressed_bytes;
    return compressed_bytes + 12;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 12) return 0;
    
    const compressed_data_t* comp = (const compressed_data_t*)input;
    if (comp->header != MAGIC_HEADER) return 0;
    if (comp->original_size > output_size) return 0;
    if (comp->compressed_size > sizeof(comp->data)) return 0;
    
    size_t decompressed_bytes = 0;
    for (size_t i = 0; i < comp->compressed_size && decompressed_bytes < comp->original_size; i++) {
        uint8_t compressed_byte = comp->data[i];
        for (int bit = 0; bit < 8 && decompressed_bytes < comp->original_size; bit++) {
            output[decompressed_bytes] = (compressed_byte & (1 << bit)) ? 0xFF : 0x00;
            decompressed_bytes++;
        }
    }
    
    return decompressed_bytes;
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
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char buffer[MAX_INPUT_SIZE + 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    memcpy(input_data, buffer, input_len);
    
    size_t compressed_size = stub_compress(input_data, input_len, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f:1\n", (double)input_len / compressed_size);
    
    printf("\nCompressed data:\n");
    print_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = stub_decompress(compressed_data, compressed_size, decompressed_data