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
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size == 0) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx + 2 < output->size) {
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
    
    if (in_idx < input->size) return -1;
    output->size = out_idx;
    return 0;
}

int compress_copy(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size > output->size) return -1;
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    return 0;
}

int decompress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size % 2 != 0) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < output->size) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > output->size) return -1;
        
        for (uint8_t i = 0; i < count; i++) {
            output->data[out_idx++] = value;
        }
    }
    
    if (in_idx < input->size) return -1;
    output->size = out_idx;
    return 0;
}

int decompress_copy(const Buffer *input, Buffer *output) {
    return compress_copy(input, output);
}

int process_compression(CompressionFunc compress, CompressionFunc decompress, 
                       const Buffer *input, Buffer *output) {
    if (compress == NULL || decompress == NULL) return -1;
    
    Buffer temp = {0};
    temp.data = malloc(MAX_OUTPUT_SIZE);
    if (temp.data == NULL) return -1;
    temp.size = MAX_OUTPUT_SIZE;
    
    if (compress(input, &temp) != 0) {
        free(temp.data);
        return -1;
    }
    
    if (decompress(&temp, output) != 0) {
        free(temp.data);
        return -1;
    }
    
    free(temp.data);
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer output = {output_data, MAX_OUTPUT_SIZE};
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input.size = strlen((char*)input_data);
    if (input.size > 0 && input_data[input.size - 1] == '\n') {
        input_data[--input.size] = '\0';
    }
    
    if (input.size == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    CompressionFunc compressors[] = {compress_rle, compress_copy};
    CompressionFunc decompressors[] = {decompress_rle, decompress_copy};
    const char *names[] = {"RLE", "Copy"};
    
    int success = 0;
    for (size_t i = 0; i < sizeof(compressors)/sizeof(compressors[0]); i++) {
        memset(output_data, 0, MAX_OUTPUT_SIZE);
        output.size = MAX_OUTPUT_SIZE;
        
        if (process_compression(compressors[i], decompressors[i], &input, &output) == 0) {
            if (input.size == output.size && memcmp(input.data, output.data, input.size) == 0) {
                printf("%s compression successful: '%.*s'\n", names[i], (int)output.size, output.data);
                success = 1;
            }
        }
    }
    
    if (!success) {
        printf("All compression methods failed\n");
        return 1;
    }
    
    return 0;
}