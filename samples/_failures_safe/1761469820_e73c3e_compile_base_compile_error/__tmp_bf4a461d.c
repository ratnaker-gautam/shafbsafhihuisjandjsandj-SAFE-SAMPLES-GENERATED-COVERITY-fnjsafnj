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

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < 2) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size - 1) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) {
            return 0;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        if (i + 1 >= input_size) {
            return 0;
        }
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    output->size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    output->size = decompress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL || buf->size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if