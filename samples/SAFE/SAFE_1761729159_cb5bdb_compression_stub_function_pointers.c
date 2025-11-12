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

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer output = {output_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(temp_input);
    if (len > 0 && temp_input[len - 1] == '\n') {
        temp_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    input.size = len;
    memcpy(input.data, temp_input, len);
    
    CompressionFunc compressors[] = {compress_rle, compress_null};
    const char *names[] = {"RLE", "Null"};
    int num_compressors = sizeof(compressors) / sizeof(compressors[0]);
    
    printf("\nCompression results:\n");
    for (int i = 0; i < num_compressors; i++) {
        output.size = 0;
        memset(output.data, 0, MAX_OUTPUT_SIZE);
        
        int result = process_compression(compressors[i], &input, &output);
        if (result != 0) {
            printf("%s compression failed\n", names[i]);
            continue;
        }
        
        printf("%s: Original=%zu bytes, Compressed=%zu bytes\n", 
               names[i], input.size, output.size);
        
        if (i == 0) {
            decompressed.size = 0;
            memset(decompressed.data, 0, MAX_OUTPUT_SIZE);
            
            if (decompress_rle(&output, &decompressed) == 0) {
                if (decompressed.size == input.size && 
                    memcmp(decompressed.data, input.data, input.size) == 0) {
                    printf("RLE decompression verified successfully\n");
                } else {
                    printf("RLE decompression verification failed\n");
                }
            }
        }
    }
    
    return 0;
}