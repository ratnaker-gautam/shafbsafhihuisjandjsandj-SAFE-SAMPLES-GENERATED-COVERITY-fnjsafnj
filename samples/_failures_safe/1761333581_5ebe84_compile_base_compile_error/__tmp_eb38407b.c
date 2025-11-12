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
    
    while (i < input_size && out_idx + 2 < output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size && out_idx < output_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    if (input_size > output_size) {
        return 0;
    }
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    uint8_t temp_output[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(compressed_size);
    if (output->data == NULL) {
        return 0;
    }
    
    memcpy(output->data, temp_output, compressed_size);
    output->size = compressed_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    uint8_t temp_output[MAX_OUTPUT_SIZE];
    size_t decompressed_size = decompress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(decompressed_size);
    if (output->data == NULL) {
        return 0;
    }
    
    memcpy(output->data, temp_output, decompressed_size);
    output->size = decompressed_size;
    return 1;
}

void print_buffer_hex(const Buffer *buffer) {
    if (buffer == NULL || buffer->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
        }
    }
    if (buffer->size % 16 != 0) {
        printf("\n");
    }
}

void free_buffer(Buffer *buffer) {
    if (buffer != NULL) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }
}

int main