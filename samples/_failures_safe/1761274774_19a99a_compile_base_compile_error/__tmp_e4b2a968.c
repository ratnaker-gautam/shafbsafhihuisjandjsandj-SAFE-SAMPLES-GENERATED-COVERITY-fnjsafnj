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

Buffer compress_data(const Buffer *input, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || compress_func == NULL) return result;
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) return result;
    
    size_t compressed_size = compress_func(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output_data;
        result.size = compressed_size;
    } else {
        free(output_data);
    }
    
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || decompress_func == NULL) return result;
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) return result;
    
    size_t decompressed_size = decompress_func(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output_data;
        result.size = decompressed_size;
    } else {
        free(output_data);
    }
    
    return result;
}

void print_buffer_hex(const Buffer *buf) {
    if (!validate_buffer(buf)) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    Buffer input = {test_data, sizeof(test_data)};
    
    printf("Original data (%zu bytes):\n", input.size);
    print_buffer_hex(&input);
    
    CompressFunc compress = run_length_encode;
    DecompressFunc decompress = run_length_decode;
    
    Buffer compressed = compress_data(&input, compress);
    if (compressed.data != NULL) {
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer_hex(&compressed);
        
        Buffer decompressed = decompress_data(&compressed, decompress);
        if (decompressed.data != NULL) {
            printf("Decompressed data (%zu bytes):\n", decompressed.size);
            print_buffer_hex(&decompressed);
            
            if (decompressed.size == input.size && 
                memcmp(decompressed.data, input.data, input.size) == 0) {
                printf("Compression/decompression successful!\n");
            } else {
                printf("Compression/decompression failed!\n");
            }
            
            free(decompressed.data);
        } else {
            printf("Decompression failed!\n");
        }
        
        free(compressed.data