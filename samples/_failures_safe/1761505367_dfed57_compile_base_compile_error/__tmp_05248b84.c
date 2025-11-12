//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size && out_index + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output[out_index++] = (uint8_t)count;
        output[out_index++] = current;
        i += count;
    }
    
    return out_index;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < input_size && out_index < output_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_index + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    return out_index;
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    uint8_t *temp_output = malloc(MAX_OUTPUT_SIZE);
    if (temp_output == NULL) {
        return 0;
    }
    
    size_t compressed_size = compress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(temp_output);
        return 0;
    }
    
    output->data = malloc(compressed_size);
    if (output->data == NULL) {
        free(temp_output);
        return 0;
    }
    
    memcpy(output->data, temp_output, compressed_size);
    output->size = compressed_size;
    free(temp_output);
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    uint8_t *temp_output = malloc(MAX_OUTPUT_SIZE);
    if (temp_output == NULL) {
        return 0;
    }
    
    size_t decompressed_size = decompress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(temp_output);
        return 0;
    }
    
    output->data = malloc(decompressed_size);
    if (output->data == NULL) {
        free(temp_output);
        return 0;
    }
    
    memcpy(output->data, temp_output, decompressed_size);
    output->size = decompressed_size;
    free(temp_output);
    return 1;
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char input_str[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");