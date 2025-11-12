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
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static int validate_buffer(const void *buf, size_t size) {
    return buf != NULL && size > 0 && size <= MAX_INPUT_SIZE;
}

static void simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (!validate_buffer(input, input_size) || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    
    comp_header_t *header = (comp_header_t*)output;
    header->header = MAGIC_HEADER;
    header->original_size = (uint32_t)input_size;
    
    uint8_t *data_start = output + sizeof(comp_header_t);
    size_t data_offset = 0;
    
    for (size_t i = 0; i < input_size && data_offset < MAX_OUTPUT_SIZE - sizeof(comp_header_t); i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE && i + j < input_size; j++) {
            compressed_byte ^= input[i + j] >> (j % 4);
        }
        data_start[data_offset++] = compressed_byte;
    }
    
    header->compressed_size = (uint32_t)data_offset;
    *output_size = sizeof(comp_header_t) + data_offset;
}

static int simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (!validate_buffer(input, input_size) || output == NULL || output_size == NULL) {
        return 0;
    }
    
    if (input_size < sizeof(comp_header_t)) {
        return 0;
    }
    
    const comp_header_t *header = (const comp_header_t*)input;
    if (header->header != MAGIC_HEADER) {
        return 0;
    }
    
    if (header->original_size > MAX_INPUT_SIZE || header->compressed_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    if (input_size != sizeof(comp_header_t) + header->compressed_size) {
        return 0;
    }
    
    const uint8_t *compressed_data = input + sizeof(comp_header_t);
    size_t out_idx = 0;
    
    for (size_t i = 0; i < header->compressed_size && out_idx < header->original_size; i++) {
        uint8_t byte = compressed_data[i];
        for (size_t j = 0; j < CHUNK_SIZE && out_idx < header->original_size; j++) {
            output[out_idx++] = byte ^ (j * 17);
        }
    }
    
    *output_size = header->original_size;
    return out_idx == header->original_size;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    simple_compress((uint8_t*)input_buffer, input_len, compressed, &compressed_size);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size;
    int decompress_ok = simple_decompress(compressed, compressed_size, decompressed, &decompressed_size);
    
    if (!decompress_ok || decompressed_size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: %.*s\n", (int)decompressed_size, decompressed);
    
    return EXIT_SUCCESS;
}