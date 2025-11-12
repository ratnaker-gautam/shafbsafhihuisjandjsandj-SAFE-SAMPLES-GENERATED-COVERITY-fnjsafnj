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
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = decompressed_size;
    return result;
}

int get_user_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, (int)max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? (int)len : 0;
}

void print_buffer_hex(const uint8_t *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        printf("(empty)\n");
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    int input_size = get_user_input(input_buffer, sizeof(input_buffer));
    
    if (input_size <= 0) {
        printf("No valid input provided.\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    printf("%s\n", input_buffer);
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("\nCompressing...\n");
    Buffer compressed = compress_data(input_buffer, (size_t)input_size, compress_func);
    
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    printf("\nDecompressing...\n");
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress_func);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        free(compressed.data