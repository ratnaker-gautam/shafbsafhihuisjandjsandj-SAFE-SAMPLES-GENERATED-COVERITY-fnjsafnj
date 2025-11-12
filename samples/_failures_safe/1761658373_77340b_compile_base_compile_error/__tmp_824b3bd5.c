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
        if (i >= output_size) return 0;
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) return 0;
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

Buffer compress_data(const Buffer *input, CompressFunc compress_fn) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || compress_fn == NULL) {
        return result;
    }
    
    result.data = malloc(MAX_OUTPUT_SIZE);
    if (result.data == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_fn(input->data, input->size, result.data, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.size = compressed_size;
    } else {
        free(result.data);
        result.data = NULL;
    }
    
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress_fn) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || decompress_fn == NULL) {
        return result;
    }
    
    result.data = malloc(MAX_OUTPUT_SIZE);
    if (result.data == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_fn(input->data, input->size, result.data, MAX_OUTPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.size = decompressed_size;
    } else {
        free(result.data);
        result.data = NULL;
    }
    
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
    Buffer input_buf = {NULL, 0};
    Buffer compressed_buf = {NULL, 0};
    Buffer decompressed_buf = {NULL, 0};
    int success = 0;
    
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
    
    input_buf.data = (uint8_t*)malloc(input_len);
    if (input_buf.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    memcpy(input_buf.data, input_str, input_len);
    input_buf.size = input_len;
    
    CompressFunc compress_fn = dummy_compress;
    DecompressFunc decompress_fn = dummy_decompress;
    
    compressed_buf = compress_data(&input_buf, compress_fn);
    if (compressed_buf.data == NULL) {
        printf("Compression failed\n");
        cleanup_buffer(&input_buf);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    
    decompressed_buf = decompress_data(&compressed_buf, decompress_fn);
    if (decompressed_buf.data == NULL) {
        printf("Decompression failed\n");
        cleanup_buffer(&input_buf);
        cleanup_buffer(&compressed_buf);
        return 1;
    }
    
    if (decompressed_buf.size == input_buf.size && 
        memcmp(decompressed_buf.data, input_buf.data, input_buf