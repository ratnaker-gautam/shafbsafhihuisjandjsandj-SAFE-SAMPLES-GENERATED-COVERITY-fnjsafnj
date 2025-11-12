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
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

int compress_null(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) {
        return -1;
    }
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (input->size > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    return 0;
}

int decompress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) {
        return -1;
    }
    if (input->size % 2 != 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_null(const Buffer *input, Buffer *output) {
    return compress_null(input, output);
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

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {compressed_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    CompressionFunc compress_funcs[] = {compress_rle, compress_null};
    CompressionFunc decompress_funcs[] = {decompress_rle, decompress_null};
    const char *method_names[] = {"RLE", "NULL"};
    
    printf("Enter data to compress (hex bytes, space separated): ");
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    char *token = strtok(line, " \n");
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) == 1 && byte <= 0xFF) {
            input.data[input.size++] = (uint8_t)byte;
        }
        token = strtok(NULL, " \n");
    }
    
    if (input.size == 0) {
        printf("No valid input data\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes): ", input.size);
    print_buffer_hex(&input);
    
    for (int i = 0; i < 2; i++) {
        compressed.size = 0;
        decompressed.size = 0;
        
        memset(compressed_data, 0, sizeof(compressed_data));
        memset(decompressed_data, 0, sizeof(decompressed_data));
        
        if (compress_funcs[i](&input, &compressed) != 0) {
            printf("Compression failed for method %s\n", method_names[i]);
            continue;
        }
        
        if (decompress_funcs[i](&compressed, &decompressed) != 0) {
            printf("Decompression failed for method %s\n", method_names[i]);