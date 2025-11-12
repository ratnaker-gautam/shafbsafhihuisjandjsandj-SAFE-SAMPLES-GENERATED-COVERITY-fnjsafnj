//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (output_size == 0 || output_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = malloc(output_size);
    if (result.data == NULL) {
        free(output_buffer);
        return result;
    }
    
    memcpy(result.data, output_buffer, output_size);
    result.size = output_size;
    free(output_buffer);
    
    return result;
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Original data size: %zu bytes\n", input_size);
    
    Buffer compressed = process_data(input_data, input_size, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = process_data(compressed.data, compressed.size, decompress_func);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        cleanup_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed data size: %zu bytes\n", decompressed.size);
    
    if (decompressed.size == input_size && memcmp(input_data, decompressed.data, input_size) == 0) {
        printf("Round-trip successful: original and decompressed data match.\n");
    } else {
        printf("Round-trip failed: data mismatch.\n");
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size && i < 50; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    cleanup_buffer(&compressed);
    cleanup_buffer(&decompressed);
    
    return 0;
}