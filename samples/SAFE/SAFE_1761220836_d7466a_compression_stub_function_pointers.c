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

size_t dummy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func, size_t output_limit) {
    Buffer result = {NULL, 0};
    if (input == NULL || func == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || output_limit > MAX_OUTPUT_SIZE) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(output_limit);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output_buffer, output_limit);
    if (output_size == 0 || output_size > output_limit) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = output_size;
    return result;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    char input_line[MAX_INPUT_SIZE * 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t line_len = strlen(input_line);
    if (line_len > 0 && input_line[line_len - 1] == '\n') {
        input_line[line_len - 1] = '\0';
        line_len--;
    }
    
    if (line_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    input_size = line_len;
    for (size_t i = 0; i < input_size; i++) {
        input_data[i] = (uint8_t)input_line[i];
    }
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    Buffer compressed = process_data(input_data, input_size, compress_func, MAX_OUTPUT_SIZE);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed.size);
    
    Buffer decompressed = process_data(compressed.data, compressed.size, decompress_func, MAX_OUTPUT_SIZE);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed back to %zu bytes\n", decompressed.size);
    
    if (decompressed.size == input_size && memcmp(input_data, decompressed.data, input_size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    printf("Original: ");
    for (size_t i = 0; i < input_size && i < 16; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    printf("Compressed: ");
    for (size_t i = 0; i < compressed.size && i < 16; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}