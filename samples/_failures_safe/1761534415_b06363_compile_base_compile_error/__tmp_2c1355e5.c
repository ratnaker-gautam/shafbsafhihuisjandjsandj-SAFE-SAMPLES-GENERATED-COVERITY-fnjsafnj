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

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 2 < output_size; i++) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < 255 && out_idx + 2 < output_size) {
            count++;
            i++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func, size_t max_output_size) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || func == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output, max_output_size);
    if (output_size == 0) {
        free(output);
        return result;
    }
    
    result.data = output;
    result.size = output_size;
    return result;
}

void print_buffer(const Buffer *buffer) {
    if (buffer == NULL || buffer->data == NULL || buffer->size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buffer->size % 16 != 0) {
        printf("\n");
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
    
    CompressFunc compress_func = simple_compress;
    DecompressFunc decompress_func = simple_decompress;
    
    Buffer compressed = process_data(input_data, input_size, compress_func, MAX_OUTPUT_SIZE);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / input_size * 100);
    
    printf("\nCompressed data:\n");
    print_buffer(&compressed);
    
    Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompress_func, MAX_OUTPUT_SIZE);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("\nDecompressed data: ");
    for (size_t i = 0; i < decompressed.size && i < sizeof(input_str); i++) {
        printf("%c",