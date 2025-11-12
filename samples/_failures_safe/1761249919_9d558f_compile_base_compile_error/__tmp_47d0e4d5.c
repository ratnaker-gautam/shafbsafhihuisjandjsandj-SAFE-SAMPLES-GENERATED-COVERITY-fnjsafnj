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
    if (compress == NULL || input == NULL || output == NULL) {
        return 0;
    }
    if (input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    uint8_t temp_output[MAX_OUTPUT_SIZE];
    size_t result_size = compress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(result_size);
    if (output->data == NULL) {
        return 0;
    }
    
    memcpy(output->data, temp_output, result_size);
    output->size = result_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL) {
        return 0;
    }
    if (input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    uint8_t temp_output[MAX_OUTPUT_SIZE];
    size_t result_size = decompress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(result_size);
    if (output->data == NULL) {
        return 0;
    }
    
    memcpy(output->data, temp_output, result_size);
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
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
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
    
    Buffer original = {(uint8_t*)user_input, input_len};
    Buffer compressed = {NULL, 0};
    Buffer decompressed = {NULL, 0};
    
    printf("Original size: %zu bytes\n", original.size);
    
    if (!compress_data(compress_func, &original, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_data(decompress_func, &compressed, &decompressed)) {
        printf("Decompression failed\n");
        cleanup_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Verification: SUCCESS - data matches original\n");
    } else {
        printf("Verification: FAILED - data corrupted\n");
    }
    
    printf("Original: %s\n", original.data);