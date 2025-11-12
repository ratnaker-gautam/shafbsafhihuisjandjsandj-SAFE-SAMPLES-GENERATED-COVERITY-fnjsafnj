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
    
    if (input_size > output_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc compress, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) return result;
    if (compress == NULL || decompress == NULL) return result;
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input, input_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) return result;
    
    uint8_t *decompressed = malloc(MAX_OUTPUT_SIZE);
    if (decompressed == NULL) return result;
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(decompressed);
        return result;
    }
    
    result.data = malloc(decompressed_size);
    if (result.data == NULL) {
        free(decompressed);
        return result;
    }
    
    memcpy(result.data, decompressed, decompressed_size);
    result.size = decompressed_size;
    free(decompressed);
    
    return result;
}

int main() {
    uint8_t test_data[] = "AAAABBBCCDAA";
    size_t test_size = sizeof(test_data) - 1;
    
    CompressFunc compressors[] = {run_length_encode, null_compress};
    DecompressFunc decompressors[] = {run_length_decode, null_decompress};
    const char *names[] = {"Run-length", "Null"};
    
    for (int i = 0; i < 2; i++) {
        printf("Testing %s compression:\n", names[i]);
        printf("Original: ");
        for (size_t j = 0; j < test_size; j++) {
            printf("%c", test_data[j]);
        }
        printf(" (%zu bytes)\n", test_size);
        
        Buffer result = process_data(test_data, test_size, compressors[i], decompressors[i]);
        
        if (result.data != NULL && result.size > 0) {
            printf("Processed: ");
            for (size_t j = 0; j < result.size; j++) {
                printf("%c", result.data[j]);
            }
            printf(" (%zu bytes)\n", result.size);
            
            if (result.size == test_size && memcmp(result.data, test_data, test_size) == 0) {
                printf("SUCCESS: Data verified\n");
            } else {
                printf("FAIL: Data mismatch\n");
            }
            
            free(result.data);
        } else {
            printf("FAIL: Processing error\n");
        }
        printf("\n");
    }
    
    return 0;
}