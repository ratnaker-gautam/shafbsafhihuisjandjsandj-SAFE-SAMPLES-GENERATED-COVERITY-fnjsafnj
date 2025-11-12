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

int get_user_input(uint8_t *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0 || buffer_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", buffer_size - 1);
    if (fgets((char*)buffer, buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? 1 : 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE] = {0};
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE] = {0};
    uint8_t decompressed_buffer[MAX_OUTPUT_SIZE] = {0};
    
    Buffer input = {input_buffer, 0};
    Buffer compressed = {compressed_buffer, MAX_OUTPUT_SIZE};
    Buffer decompressed = {decompressed_buffer, MAX_OUTPUT_SIZE};
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    if (!get_user_input(input_buffer, sizeof(input_buffer))) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    input.size = strlen((char*)input_buffer);
    
    printf("Original data: %s\n", input_buffer);
    printf("Original size: %zu bytes\n", input.size);
    
    if (!compress_data(compress_func, &input, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_data(decompress_func, &compressed, &decompressed)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: %s\n", decompressed_buffer);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (input.size != decompressed.size || memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Error: Data mismatch after compression/decompression\n");
        return 1;
    }
    
    printf("Compression/decompression successful\n");
    return 0;
}