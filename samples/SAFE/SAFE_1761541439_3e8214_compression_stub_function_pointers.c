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

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
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

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func, size_t max_output) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || func == NULL || max_output == 0) {
        return result;
    }
    
    if (input_size > MAX_INPUT_SIZE) {
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

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", test_size);
    for (size_t i = 0; i < test_size; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\n\n");
    
    CompressFunc compressors[] = {compress_rle};
    DecompressFunc decompressors[] = {decompress_rle};
    const char *names[] = {"RLE"};
    
    for (size_t i = 0; i < 1; i++) {
        printf("Testing %s compression:\n", names[i]);
        
        Buffer compressed = process_data(test_data, test_size, compressors[i], MAX_OUTPUT_SIZE);
        if (compressed.data == NULL) {
            printf("Compression failed\n\n");
            continue;
        }
        
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer(&compressed);
        
        Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompressors[i], MAX_OUTPUT_SIZE);
        if (decompressed.data == NULL) {
            printf("Decompression failed\n\n");
            free(compressed.data);
            continue;
        }
        
        printf("Decompressed data (%zu bytes):\n", decompressed.size);
        print_buffer(&decompressed);
        
        if (decompressed.size == test_size && memcmp(test_data, decompressed.data, test_size) == 0) {
            printf("Verification: SUCCESS\n");
        } else {
            printf("Verification: FAILED\n");
        }
        
        printf("Compression ratio: %.2f%%\n\n", (float)compressed.size / test_size * 100);
        
        free(compressed.data);
        free(decompressed.data);
    }
    
    return 0;
}