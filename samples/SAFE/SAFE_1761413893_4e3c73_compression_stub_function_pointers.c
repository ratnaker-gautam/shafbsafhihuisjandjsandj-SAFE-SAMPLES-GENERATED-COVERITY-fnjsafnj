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
        if (i >= output_size) {
            return 0;
        }
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) {
            return 0;
        }
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
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
    if (decompress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t result = decompress(input->data, input->size, output->data, output->size);
    if (result == 0 || result > output->size) {
        return 0;
    }
    
    output->size = result;
    return 1;
}

void process_compression_roundtrip(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    Buffer input = {input_buffer, 0};
    Buffer compressed = {compressed_buffer, MAX_OUTPUT_SIZE};
    Buffer decompressed = {decompressed_buffer, MAX_INPUT_SIZE};
    
    const char *test_string = "This is a test string for compression demonstration.";
    size_t len = strlen(test_string);
    
    if (len >= MAX_INPUT_SIZE) {
        printf("Test string too long\n");
        return;
    }
    
    memcpy(input.data, test_string, len);
    input.size = len;
    
    printf("Original: %s\n", input.data);
    printf("Original size: %zu\n", input.size);
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    if (!compress_data(compress_func, &input, &compressed)) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    
    if (!decompress_data(decompress_func, &compressed, &decompressed)) {
        printf("Decompression failed\n");
        return;
    }
    
    printf("Decompressed: %s\n", decompressed.data);
    printf("Decompressed size: %zu\n", decompressed.size);
    
    if (input.size != decompressed.size) {
        printf("Size mismatch\n");
        return;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Data mismatch\n");
        return;
    }
    
    printf("Roundtrip successful\n");
}

int main(void) {
    process_compression_roundtrip();
    return 0;
}