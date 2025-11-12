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
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
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

Buffer compress_data(const Buffer *input, CompressFunc compress) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || compress == NULL) return result;
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) return result;
    
    size_t compressed_size = compress(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output_data;
        result.size = compressed_size;
    } else {
        free(output_data);
    }
    
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || decompress == NULL) return result;
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) return result;
    
    size_t decompressed_size = decompress(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output_data;
        result.size = decompressed_size;
    } else {
        free(output_data);
    }
    
    return result;
}

void process_compression_demo(void) {
    uint8_t test_data[] = "AAAABBBCCDAA";
    size_t test_size = strlen((char*)test_data);
    
    Buffer input = {test_data, test_size};
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu\n", test_size);
    
    Buffer compressed = compress_data(&input, compress_func);
    if (compressed.data != NULL) {
        printf("Compressed size: %zu\n", compressed.size);
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / test_size) * 100);
        
        Buffer decompressed = decompress_data(&compressed, decompress_func);
        if (decompressed.data != NULL) {
            printf("Decompressed data: ");
            for (size_t i = 0; i < decompressed.size; i++) {
                printf("%c", decompressed.data[i]);
            }
            printf("\n");
            printf("Decompression successful: %s\n", 
                   decompressed.size == test_size && memcmp(decompressed.data, test_data, test_size) == 0 ? "Yes" : "No");
            free(decompressed.data);
        }
        free(compressed.data);
    }
}

int main(void) {
    process_compression_demo();
    return 0;
}