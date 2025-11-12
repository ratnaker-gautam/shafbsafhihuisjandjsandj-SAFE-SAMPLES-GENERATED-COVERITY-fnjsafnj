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

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output);
        return result;
    }
    
    result.data = output;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_OUTPUT_SIZE || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_INPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input, input_size, output, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_INPUT_SIZE) {
        free(output);
        return result;
    }
    
    result.data = output;
    result.size = decompressed_size;
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
    char input_str[MAX_INPUT_SIZE];
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
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    Buffer compressed = compress_data((uint8_t*)input_str, input_len, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress_func);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        cleanup_buffer(&compressed);
        return 1;
    }
    
    if (decompressed.size == input_len && memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Round-trip successful\n");
        printf("Decompressed text: %s\n", decompressed.data);
    } else {
        printf("Round-trip verification failed\n");
    }
    
    cleanup_buffer(&compressed);
    cleanup_buffer(&decompressed);
    
    return 0;
}