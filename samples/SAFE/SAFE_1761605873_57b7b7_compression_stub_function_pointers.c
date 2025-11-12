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

typedef int (*CompressionFunc)(const Buffer *input, Buffer *output);

int compress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) {
        return -1;
    }
    if (input->size == 0) {
        output->size = 0;
        return 0;
    }
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) {
        return -1;
    }
    if (input->size == 0) {
        output->size = 0;
        return 0;
    }
    if (input->size % 2 != 0 || input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

int process_compression(CompressionFunc func, const Buffer *input, Buffer *output) {
    if (func == NULL || input == NULL || output == NULL) {
        return -1;
    }
    return func(input, output);
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {output_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    int c;
    size_t pos = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (pos < MAX_INPUT_SIZE - 1) {
            input.data[pos++] = (uint8_t)c;
        }
    }
    input.size = pos;
    
    if (input.size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    CompressionFunc compress_func = compress_rle;
    CompressionFunc decompress_func = decompress_rle;
    
    printf("Original data (%zu bytes): ", input.size);
    print_buffer_hex(&input);
    
    if (process_compression(compress_func, &input, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    if (process_compression(decompress_func, &compressed, &decompressed) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (decompressed.size == input.size && 
        memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Compression/Decompression successful.\n");
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / input.size) * 100.0);
    } else {
        printf("Data mismatch after decompression.\n");
        return 1;
    }
    
    return 0;
}