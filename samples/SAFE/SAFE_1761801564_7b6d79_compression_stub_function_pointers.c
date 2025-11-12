//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t dummy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) return 0;
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) return 0;
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

int get_user_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return -1;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    char temp_buffer[MAX_INPUT_SIZE];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(temp_buffer);
    if (len > 0 && temp_buffer[len - 1] == '\n') {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= max_size) {
        return -1;
    }
    
    memcpy(buffer, temp_buffer, len);
    buffer[len] = '\0';
    return (int)len;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    if (compress == NULL || decompress == NULL) {
        printf("Error: Invalid compression functions\n");
        return;
    }
    
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    int input_len = get_user_input(input, sizeof(input));
    if (input_len <= 0) {
        printf("Error: Invalid input\n");
        return;
    }
    
    printf("Original text: %s\n", input);
    printf("Original size: %d bytes\n", input_len);
    
    size_t compressed_size = compress(input, (size_t)input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != (size_t)input_len) {
        printf("Error: Decompression failed\n");
        return;
    }
    
    decompressed[decompressed_size] = '\0';
    printf("Decompressed text: %s\n", decompressed);
    
    if (memcmp(input, decompressed, (size_t)input_len) == 0) {
        printf("Verification: Success - data matches original\n");
    } else {
        printf("Verification: Failed - data corrupted\n");
    }
}

int main() {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Demo\n");
    printf("======================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}