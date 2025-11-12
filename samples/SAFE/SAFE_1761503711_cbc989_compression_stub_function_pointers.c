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
        output[i] = input[i] + 1;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i++) {
        output[i] = input[i] - 1;
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

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    char user_input[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen(user_input);
    if (input_length > 0 && user_input[input_length - 1] == '\n') {
        user_input[input_length - 1] = '\0';
        input_length--;
    }
    
    if (input_length == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    memcpy(input_data, user_input, input_length);
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    Buffer compressed = compress_data(input_data, input_length, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_length);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress_func);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size && i < MAX_INPUT_SIZE; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input_length && memcmp(input_data, decompressed.data, input_length) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}