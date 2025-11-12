//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t copy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size > output_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t copy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return copy_compress(input, input_size, output, output_size);
}

int compress_buffer(const Buffer *input, Buffer *output, CompressFunc compress_func) {
    if (input == NULL || output == NULL || compress_func == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    if (input->size > MAX_INPUT_SIZE) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = compress_func(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_buffer(const Buffer *input, Buffer *output, DecompressFunc decompress_func) {
    if (input == NULL || output == NULL || decompress_func == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    if (input->size > MAX_INPUT_SIZE) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = decompress_func(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

void print_buffer_hex(const Buffer *buffer) {
    if (buffer == NULL || buffer->data == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

void cleanup_buffer(Buffer *buffer) {
    if (buffer != NULL && buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }
}

int main() {
    CompressFunc compress_funcs[] = {run_length_encode, copy_compress};
    DecompressFunc decompress_funcs[] = {run_length_decode, copy_decompress};
    const char *method_names[] = {"Run-length encoding", "Copy (no compression)"};
    const int num_methods = 2;
    
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    char input_text[MAX_INPUT_SIZE + 1];
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error