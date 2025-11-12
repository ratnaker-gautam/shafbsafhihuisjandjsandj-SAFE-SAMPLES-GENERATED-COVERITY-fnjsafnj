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
    memcpy(output, input, input_size);
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL) {
        return 0;
    }
    if (input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    if (output->data == NULL || output->size < input->size) {
        return 0;
    }
    
    size_t result = compress(input->data, input->size, output->data, output->size);
    if (result == 0 || result > output->size) {
        return 0;
    }
    output->size = result;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL) {
        return 0;
    }
    if (input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    if (output->data == NULL || output->size < input->size) {
        return 0;
    }
    
    size_t result = decompress(input->data, input->size, output->data, output->size);
    if (result == 0 || result > output->size) {
        return 0;
    }
    output->size = result;
    return 1;
}

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {compressed_data, MAX_OUTPUT_SIZE};
    Buffer decompressed = {decompressed_data, MAX_OUTPUT_SIZE};
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    char *token = strtok(line, " ");
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) == 1 && byte <= 0xFF) {
            input.data[input.size++] = (uint8_t)byte;
        }
        token = strtok(NULL, " ");
    }
    
    if (input.size == 0) {
        printf("No valid input data\n");
        return 1;
    }
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Original data (%zu bytes): ", input.size);
    print_buffer(&input);
    
    if (!compress_data(compress_func, &input, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer(&compressed);
    
    if (!decompress_data(decompress_func, &compressed, &decompressed)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer(&decompressed);
    
    if (input.size == decompressed.size && 
        memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
        return 1;
    }
    
    return 0;
}