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

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static int validate_input(const char *input, size_t input_len) {
    if (input == NULL) return 0;
    if (input_len == 0) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_compress(const uint8_t *input, size_t input_len, compressed_data_t *output) {
    output->header = MAGIC_HEADER;
    output->original_size = (uint32_t)input_len;
    output->compressed_size = 0;
    
    for (size_t i = 0; i < input_len && output->compressed_size < sizeof(output->data); i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        size_t chunk_end = (i + CHUNK_SIZE < input_len) ? i + CHUNK_SIZE : input_len;
        
        for (size_t j = i; j < chunk_end; j++) {
            compressed_byte ^= input[j];
        }
        
        output->data[output->compressed_size] = compressed_byte;
        output->compressed_size++;
    }
}

static int simple_decompress(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    if (compressed->header != MAGIC_HEADER) return 0;
    if (compressed->original_size > output_size) return 0;
    if (compressed->compressed_size > sizeof(compressed->data)) return 0;
    
    for (size_t i = 0; i < compressed->original_size; i++) {
        size_t chunk_index = i / CHUNK_SIZE;
        if (chunk_index >= compressed->compressed_size) {
            output[i] = 0;
        } else {
            output[i] = compressed->data[chunk_index];
        }
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    uint8_t input_data[MAX_INPUT_SIZE] = {0};
    compressed_data_t compressed;
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, input_buffer, input_len);
    
    simple_compress(input_data, input_len, &compressed);
    
    printf("Original size: %u bytes\n", compressed.original_size);
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    printf("Compression ratio: %.2f:1\n", 
           (float)compressed.original_size / (float)compressed.compressed_size);
    
    if (!simple_decompress(&compressed, decompressed, sizeof(decompressed))) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < compressed.original_size && i < sizeof(decompressed); i++) {
        putchar(decompressed[i]);
    }
    printf("\n");
    
    if (memcmp(input_data, decompressed, compressed.original_size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}