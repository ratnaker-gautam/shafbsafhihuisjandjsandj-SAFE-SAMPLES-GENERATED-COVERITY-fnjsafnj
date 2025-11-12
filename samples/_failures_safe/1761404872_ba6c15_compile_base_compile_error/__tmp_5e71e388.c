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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 < output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    if (input->size > MAX_INPUT_SIZE) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    if (input->size > MAX_OUTPUT_SIZE) return 0;
    
    output->data = malloc(MAX_INPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = decompress(input->data, input->size, output->data, MAX_INPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const char* name) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    uint8_t test_data[] = "AAAABBBCCDAA";
    original.data = test_data;
    original.size = strlen((char*)test_data);
    
    printf("\n%s Compression:\n", name);
    printf("Original: %s\n", original.data);
    printf("Original size: %zu\n", original.size);
    
    if (compress_data(compress, &original, &compressed)) {
        printf("Compressed size: %zu\n", compressed.size);
        
        if (decompress_data(decompress, &compressed, &decompressed)) {
            printf("Decompressed: %s\n", decompressed.data);
            printf("Decompressed size: %zu\n", decompressed.size);
            
            if (original.size == decompressed.size && 
                memcmp(original.data, decompressed.data, original.size) == 0) {
                printf("Verification: SUCCESS\n");
            } else {
                printf("Verification: FAILED\n");
            }
            
            free(decompressed.data);
        } else {
            printf("Decompression failed\n");
        }
        
        free(compressed.data);
    } else {
        printf("Compression failed\n