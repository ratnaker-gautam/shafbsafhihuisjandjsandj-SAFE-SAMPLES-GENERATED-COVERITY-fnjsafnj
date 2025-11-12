//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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
        
        output[out_idx++] = (uint8_t)count;
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
    
    size_t copy_size = input_size;
    if (copy_size > output_size) copy_size = output_size;
    
    memcpy(output, input, copy_size);
    return copy_size;
}

size_t copy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return copy_compress(input, input_size, output, output_size);
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc compress, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) return result;
    if (compress == NULL || decompress == NULL) return result;
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input, input_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) return result;
    
    uint8_t *decompressed = malloc(MAX_INPUT_SIZE);
    if (decompressed == NULL) return result;
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    
    if (decompressed_size == input_size && memcmp(input, decompressed, input_size) == 0) {
        result.data = malloc(compressed_size);
        if (result.data != NULL) {
            memcpy(result.data, compressed, compressed_size);
            result.size = compressed_size;
        }
    }
    
    free(decompressed);
    return result;
}

void print_buffer(const Buffer *buffer) {
    if (buffer == NULL || buffer->data == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    CompressFunc compressors[] = {run_length_encode, copy_compress};
    DecompressFunc decompressors[] = {run_length_decode, copy_decompress};
    const char *names[] = {"Run-length encoding", "Copy compression"};
    
    for (int i = 0; i < 2; i++) {
        printf("Testing %s:\n", names[i]);
        printf("Original data (%zu bytes): ", test_size);
        for (size_t j = 0; j < test_size; j++) {
            printf("%02X ", test_data[j]);
        }
        printf("\n");
        
        Buffer result = process_data(test_data, test_size, compressors[i], decompressors[i]);
        
        if (result.data != NULL) {
            printf("Compressed data (%zu bytes): ", result.size);
            print_buffer(&result);
            printf("Compression ratio: %.2f\n", (float)result.size / test_size);
            free(result.data);
        } else {
            printf("Compression failed\n");