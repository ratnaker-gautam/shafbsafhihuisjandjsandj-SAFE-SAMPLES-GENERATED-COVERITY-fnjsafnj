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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
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
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        if (i + 1 >= input_size) break;
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    
    size_t result_size = compress(input->data, input->size, output->data, output->size);
    if (result_size == 0 || result_size > output->size) return 0;
    
    output->size = result_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    
    size_t result_size = decompress(input->data, input->size, output->data, output->size);
    if (result_size == 0 || result_size > output->size) return 0;
    
    output->size = result_size;
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

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {compressed_data, MAX_OUTPUT_SIZE};
    Buffer decompressed = {decompressed_data, MAX_INPUT_SIZE};
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \n");
    size_t count = 0;
    
    while (token != NULL && count < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) != 1) {
            printf("Invalid hex value: %s\n", token);
            return 1;
        }
        if (value > 0xFF) {
            printf("Value out of range: %s\n", token);
            return 1;
        }
        input.data[count++] = (uint8_t)value;
        token = strtok(NULL, " \n");
    }
    
    input.size = count;
    
    if (input.size == 0) {
        printf("No valid input data provided\n");
        return 1;
    }
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(&input);
    
    if (!compress_data(compress_func, &input, &compressed)) {