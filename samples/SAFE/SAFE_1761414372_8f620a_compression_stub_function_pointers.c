//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

int get_user_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return -1;
    }
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    if (fgets((char*)buffer, max_size, stdin) == NULL) {
        return -1;
    }
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    return len;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    int input_len = get_user_input(input, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        printf("Invalid input or empty string.\n");
        return;
    }
    
    size_t compressed_size = compress(input, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return;
    }
    
    printf("Original size: %d bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size < (size_t)input_len) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size/input_len) * 100);
    } else {
        printf("No compression achieved.\n");
    }
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != (size_t)input_len) {
        printf("Decompression failed.\n");
        return;
    }
    
    if (memcmp(input, decompressed, input_len) == 0) {
        printf("Decompression verified successfully.\n");
        printf("Decompressed text: %s\n", decompressed);
    } else {
        printf("Decompression verification failed.\n");
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