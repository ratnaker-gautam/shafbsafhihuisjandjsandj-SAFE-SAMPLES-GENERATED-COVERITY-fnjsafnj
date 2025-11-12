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
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || compress_func == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t compressed_size = compress_func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(compressed_size);
        if (result.data != NULL) {
            memcpy(result.data, output, compressed_size);
            result.size = compressed_size;
        }
    }
    
    free(output);
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || decompress_func == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t decompressed_size = decompress_func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(decompressed_size);
        if (result.data != NULL) {
            memcpy(result.data, output, decompressed_size);
            result.size = decompressed_size;
        }
    }
    
    free(output);
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

void free_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input_data, input_str, input_len);
    size_t input_size = input_len;
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_buffer_hex(input_data, input_size);
    
    Buffer compressed = compress