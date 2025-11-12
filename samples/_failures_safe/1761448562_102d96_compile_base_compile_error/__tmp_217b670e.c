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
    }
    
    size_t result = compress_func(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (result == 0 || result > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    output->size = result;
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
    }
    
    size_t result = decompress_func(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (result == 0 || result > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    output->size = result;
    return 1;
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    input.data = malloc(input_len);
    if (input.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    input.size = input_len;
    memcpy(input.data, user_input, input_len);
    
    CompressFunc compress = dummy_compress;
    DecompressFunc decompress = dummy_decompress;
    
    printf("Original size: %zu bytes\n", input.size);
    
    if (!compress_data(&input, &compressed, compress)) {
        printf("Compression failed\n");
        cleanup_buffer(&input);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_data(&compressed, &decompressed, decompress)) {
        printf("Decompression failed\n");
        cleanup_buffer(&input);
        cleanup_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (input.size == decompressed.size && memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Verification: SUCCESS - data matches original\n");
    } else {
        printf("Verification: FAILED - data corrupted\n");
    }
    
    cleanup_buffer(&input);
    cleanup_buffer(&compressed);
    cleanup_buffer(&decomp