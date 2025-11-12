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

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
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

size_t copy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t copy_size = (input_size < output_size) ? input_size : output_size;
    memcpy(output, input, copy_size);
    return copy_size;
}

size_t copy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return copy_compress(input, input_size, output, output_size);
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

Buffer compress_data(const Buffer *input, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || compress_func == NULL) return result;
    
    result.data = malloc(MAX_OUTPUT_SIZE);
    if (result.data == NULL) return result;
    
    result.size = compress_func(input->data, input->size, result.data, MAX_OUTPUT_SIZE);
    
    if (result.size == 0) {
        free(result.data);
        result.data = NULL;
    }
    
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || decompress_func == NULL) return result;
    
    result.data = malloc(MAX_OUTPUT_SIZE);
    if (result.data == NULL) return result;
    
    result.size = decompress_func(input->data, input->size, result.data, MAX_OUTPUT_SIZE);
    
    if (result.size == 0) {
        free(result.data);
        result.data = NULL;
    }
    
    return result;
}

void process_compression_demo(void) {
    uint8_t test_data[] = "AAAABBBCCDAA";
    Buffer input = {test_data, sizeof(test_data) - 1};
    
    CompressFunc compressors[] = {run_length_encode, copy_compress};
    DecompressFunc decompressors[] = {run_length_decode, copy_decompress};
    const char *names[] = {"Run-length", "Copy"};
    
    for (int i = 0; i < 2; i++) {
        printf("Testing %s compression:\n", names[i]);
        printf("Original: %s\n", test_data);
        
        Buffer compressed = compress_data(&input, compressors[i]);
        if (compressed.data == NULL) {
            printf("Compression failed\n");
            continue;
        }
        
        printf("Compressed size: %zu\n", compressed.size);
        
        Buffer decompressed = decompress_data(&compressed, decompressors[i]);
        if (decompressed.data == NULL) {
            printf("Decompression failed\n");
            free(compressed.data);
            continue;
        }
        
        printf("Decompressed: ");
        for (size_t j = 0; j < decompressed.size; j++) {
            printf("%c", decompressed.data[j]);
        }
        printf("\n\n");
        
        free(compressed.data);
        free(decompressed.data);
    }
}

int main(void) {
    process_compression_demo();
    return 0;
}