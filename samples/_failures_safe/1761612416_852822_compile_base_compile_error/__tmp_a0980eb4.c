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

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

Buffer compress_data(const Buffer *input, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || compress_func == NULL) {
        return result;
    }
    
    result.data = malloc(MAX_OUTPUT_SIZE);
    if (result.data == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input->data, input->size, result.data, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.size = compressed_size;
    } else {
        free(result.data);
        result.data = NULL;
    }
    
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || decompress_func == NULL) {
        return result;
    }
    
    result.data = malloc(MAX_OUTPUT_SIZE);
    if (result.data == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input->data, input->size, result.data, MAX_OUTPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.size = decompressed_size;
    } else {
        free(result.data);
        result.data = NULL;
    }
    
    return result;
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
    char input_str[MAX_INPUT_SIZE];
    uint8_t input_copy[MAX_INPUT_SIZE];
    Buffer input_buf = {NULL, 0};
    Buffer compressed_buf = {NULL, 0};
    Buffer decompressed_buf = {NULL, 0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE - 1) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input_copy, input_str, input_len);
    input_buf.data = input_copy;
    input_buf.size = input_len;
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    compressed_buf = compress_data(&input_buf, compress_func);
    if (compressed_buf.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    decompressed_buf = decompress_data(&compressed_buf, decompress_func);
    if (decompressed_buf.data == NULL) {
        printf("Decompression failed\n");
        free(compressed_buf.data);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    printf("Decompressed size: %zu bytes\n", decompressed_buf.size);
    
    printf("Original data (hex): ");
    print_buffer_hex(&input_buf);
    
    printf("Compressed data (hex