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

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) break;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

int compress_data(const Buffer *input, Buffer *output, CompressFunc compress) {
    if (!validate_buffer(input) || output == NULL || compress == NULL) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    
    if (output->size == 0 || output->size > MAX_OUTPUT_SIZE) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_data(const Buffer *input, Buffer *output, DecompressFunc decompress) {
    if (!validate_buffer(input) || output == NULL || decompress == NULL) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = decompress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    
    if (output->size == 0 || output->size > MAX_OUTPUT_SIZE) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

void process_compression(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    original.data = malloc(MAX_INPUT_SIZE);
    if (original.data == NULL) return;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)original.data, MAX_INPUT_SIZE, stdin) == NULL) {
        free(original.data);
        return;
    }
    
    original.size = strlen((char*)original.data);
    if (original.size > 0 && original.data[original.size - 1] == '\n') {
        original.data[--original.size] = '\0';
    }
    
    if (original.size == 0) {
        printf("No input provided.\n");
        free(original.data);
        return;
    }
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    if (!compress_data(&original, &compressed, compress_func)) {
        printf("Compression failed.\n");
        free(original.data);
        return;
    }
    
    printf("Original size: %zu bytes\n", original.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_data(&compressed, &decompressed, decompress_func)) {
        printf("Decompression failed.\n");
        free(original.data);
        free(compressed.data);
        return;
    }
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Decompression successful. Data matches original.\n");
    } else {
        printf("Decompression failed. Data mismatch.\n");
    }
    
    free(original.data);
    free(compressed.data);
    free(decomp