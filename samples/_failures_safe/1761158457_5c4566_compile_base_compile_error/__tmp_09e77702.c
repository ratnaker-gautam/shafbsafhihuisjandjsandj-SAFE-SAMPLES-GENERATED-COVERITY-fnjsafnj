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
    if (func == NULL) {
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
    Buffer output = {output_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    char *token = strtok(line, " \n");
    size_t count = 0;
    
    while (token != NULL && count < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) != 1) {
            printf("Invalid hex input\n");
            return 1;
        }
        input_data[count++] = (uint8_t)value;
        token = strtok(NULL, " \n");
    }
    input.size = count;
    
    if (input.size == 0) {
        printf("No input data\n");
        return 1;
    }
    
    CompressionFunc compressors[] = {compress_rle, compress_null};
    CompressionFunc decompressors[] = {decompress_rle, compress_null};
    const char *names[] = {"RLE", "Null"};
    
    for (int i = 0; i < 2; i++) {
        printf("\n%s Compression:\n", names[i]);
        
        output.size = 0;
        if (process_compression(compressors[i], &input, &output) != 0) {
            printf("Compression failed\n");
            continue;
        }
        
        printf("Compressed (%zu bytes): ", output.size);
        print_buffer_hex(&output);
        
        decompressed.size = 0;
        if (process_compression(decompressors[i], &output, &decompressed) != 0) {
            printf("Decompression failed\n");
            continue;
        }
        
        printf("Decomp