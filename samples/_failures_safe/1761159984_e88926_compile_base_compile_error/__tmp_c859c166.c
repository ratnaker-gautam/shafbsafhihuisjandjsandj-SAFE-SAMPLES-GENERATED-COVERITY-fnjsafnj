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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compress_buffer(const Buffer *input, Buffer *output, CompressFunc compress_func) {
    if (input == NULL || output == NULL || compress_func == NULL) {
        return 0;
    }
    
    if (input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    output->size = compress_func(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_buffer(const Buffer *input, Buffer *output, DecompressFunc decompress_func) {
    if (input == NULL || output == NULL || decompress_func == NULL) {
        return 0;
    }
    
    if (input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    output->size = decompress_func(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

void print_buffer_hex(const Buffer *buffer) {
    if (buffer == NULL || buffer->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer original, compressed, decompressed;
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    
    original.data = test_data;
    original.size = sizeof(test_data);
    
    printf("Original data (%zu bytes): ", original.size);
    print_buffer_hex(&original);
    
    CompressFunc compress = run_length_encode;
    DecompressFunc decompress = run_length_decode;
    
    if (compress_buffer(&original, &compressed, compress)) {
        printf("Compressed data (%zu bytes): ", compressed.size);
        print_buffer_hex(&compressed);
        
        if (decompress_buffer(&compressed, &decompressed, decompress)) {
            printf("Decompressed data (%zu bytes): ", decompressed.size);
            print_buffer_hex(&decompressed);
            
            if (decompressed.size == original.size && 
                memcmp(decompressed.data, original.data, original.size) == 0) {
                printf("Compression/decompression successful!\n");
            } else {
                printf("Compression/decompression failed!\n");
            }
            
            free(decompressed.data);
        } else {
            printf("Decompression failed!\n");
        }
        
        free(compressed.data