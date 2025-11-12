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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
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
    
    if (input->size > MAX_INPUT_SIZE) {
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
    
    if (input->size > MAX_INPUT_SIZE) {
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

int buffers_equal(const Buffer *a, const Buffer *b) {
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        return 0;
    }
    
    if (a->size != b->size) {
        return 0;
    }
    
    return memcmp(a->data, b->data, a->size) == 0;
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main(void) {
    CompressFunc compressors[] = {run_length_encode, null_compress};
    DecompressFunc decompressors[] = {run_length_decode, null_decompress};
    const char *names[] = {"Run-length", "Null"};
    size_t num_methods = sizeof(compressors) / sizeof(compressors[0]);
    
    uint8_t test_data[] = "AAAABBBCCDAA";
    Buffer original = {test_data, sizeof(test_data)