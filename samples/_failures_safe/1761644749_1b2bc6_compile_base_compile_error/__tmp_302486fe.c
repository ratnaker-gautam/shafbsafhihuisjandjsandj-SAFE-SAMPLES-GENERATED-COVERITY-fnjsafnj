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
        output[i] = input[i] + 1;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] - 1;
    }
    return input_size;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

int compress_data(const Buffer *input, Buffer *output, CompressFunc compress_func) {
    if (!validate_buffer(input) || output == NULL || compress_func == NULL) {
        return 0;
    }
    
    if (output->data == NULL) {
        output->data = malloc(MAX_OUTPUT_SIZE);
        if (output->data == NULL) {
            return 0;
        }
        output->size = MAX_OUTPUT_SIZE;
    }
    
    size_t result_size = compress_func(input->data, input->size, output->data, output->size);
    if (result_size == 0 || result_size > output->size) {
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

int decompress_data(const Buffer *input, Buffer *output, DecompressFunc decompress_func) {
    if (!validate_buffer(input) || output == NULL || decompress_func == NULL) {
        return 0;
    }
    
    if (output->data == NULL) {
        output->data = malloc(MAX_OUTPUT_SIZE);
        if (output->data == NULL) {
            return 0;
        }
        output->size = MAX_OUTPUT_SIZE;
    }
    
    size_t result_size = decompress_func(input->data, input->size, output->data, output->size);
    if (result_size == 0 || result_size > output->size) {
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

void print_buffer_hex(const Buffer *buf) {
    if (!validate_buffer(buf)) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02x", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer input_buf = {input_data, 0};
    Buffer compressed_buf = {NULL, 0};
    Buffer decompressed_buf = {NULL, 0};
    int ret = 1;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_buf.size = strlen((char*)input_data);
    if (input_buf.size > 0 && input_data[input_buf.size - 1] == '\n') {
        input_data[input_buf.size - 1] = '\0';
        input_buf.size--;
    }
    
    if (input_buf.size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Original data (%zu bytes): ", input_buf.size);
    print_buffer_hex(&input_buf);
    
    if (!compress_data(&input_buf, &compressed_buf, compress_func)) {
        printf("Compression failed\n");
        goto cleanup;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_buf.size);
    print_buffer_hex(&compressed_buf);
    
    if (!decompress_data(&compressed_buf, &decompressed_buf, decompress_func)) {
        printf("Decompression failed\n");
        goto cleanup;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_buf.size);
    print_buffer_hex(&decompressed_buf);
    
    if (input_buf.size == decompressed_buf.size && 
        memcmp(input_buf.data, decompressed_buf.data, input_buf.size) == 0) {
        printf("Round-trip successful\n");
        ret