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

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func, size_t max_output) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || func == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(max_output);
    if (output == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output, max_output);
    if (output_size == 0) {
        free(output);
        return result;
    }
    
    result.data = output;
    result.size = output_size;
    return result;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    CompressFunc compress_funcs[] = {run_length_encode};
    DecompressFunc decompress_funcs[] = {run_length_decode};
    const char *method_names[] = {"Run-Length Encoding"};
    size_t num_methods = 1;
    
    for (size_t i = 0; i < num_methods; i++) {
        printf("\nMethod: %s\n", method_names[i]);
        
        Buffer compressed = process_data(input_data, input_size, compress_funcs[i], MAX_OUTPUT_SIZE);
        if (compressed.data == NULL) {
            printf("Compression failed.\n");
            continue;
        }
        
        printf("Original size: %zu bytes\n", input_size);
        printf("Compressed size: %zu bytes\n", compressed.size);
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / input_size) * 100.0);
        
        Buffer decompressed = process_data(compressed.data, compressed.size, 
                                         (CompressFunc)decompress_funcs[i], MAX_INPUT_SIZE);
        
        if (decompressed.data != NULL && decompressed.size == input_size && 
            memcmp(input_data, decompressed.data, input_size) == 0) {
            printf("Decompression successful - data verified.\n");
        } else {
            printf("Decompression failed - data corrupted.\n");
        }
        
        if (compressed.data) free(compressed.data);
        if (decompressed.data) free(decompressed.data);
    }
    
    return 0;
}