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
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    size_t result_size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    size_t result_size = decompress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    output->size = result_size;
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
    
    char user_input[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
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
    
    input.data = (uint8_t*)malloc(input_len);
    if (input.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    input.size = input_len;
    memcpy(input.data, user_input, input_len);
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    if (!compress_data(compress_func, &input, &compressed)) {
        printf("Compression failed\n");
        cleanup_buffer(&input);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_data(decompress_func, &compressed, &decompressed)) {
        printf("Decompression failed\n");
        cleanup_buffer(&input);
        cleanup_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (decompressed.size == input.size && 
        memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("