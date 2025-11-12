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

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

Buffer compress_data(const Buffer *input, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    if (!validate_buffer(input) || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output_data);
        return result;
    }
    
    result.data = output_data;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    if (!validate_buffer(input) || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(output_data);
        return result;
    }
    
    result.data = output_data;
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
    
    Buffer input_buf;
    input_buf.data = (uint8_t*)malloc(input_len);
    if (input_buf.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    memcpy(input_buf.data, input_str, input_len);
    input_buf.size = input_len;
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Original size: %zu bytes\n", input_buf.size);
    
    Buffer compressed = compress_data(&input_buf, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        cleanup_buffer(&input_buf);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress_data(&compressed, decompress_func);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        cleanup_buffer(&input_buf);
        cleanup_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (decompressed.size == input_buf.size && 
        memcmp(decompressed.data, input_buf.data, input_buf.size) == 0) {
        printf("Round-trip successful!\n");
    } else {
        printf("Round-trip failed!\n");
    }
    
    cleanup_buffer(&input_buf);
    cleanup_buffer(&compressed);
    cleanup_buffer(&decompressed);
    
    return 0;
}