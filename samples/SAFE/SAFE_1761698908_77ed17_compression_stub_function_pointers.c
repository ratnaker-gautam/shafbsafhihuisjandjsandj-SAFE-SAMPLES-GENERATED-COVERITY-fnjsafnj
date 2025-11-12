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
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    return input_size;
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (output_size == 0 || output_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = output_size;
    return result;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size && i < 32; i++) {
        printf("%02x ", data[i]);
    }
    if (size > 32) {
        printf("...");
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
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
    
    memcpy(input_data, input_str, input_len);
    
    CompressFunc compressors[] = {dummy_compress};
    DecompressFunc decompressors[] = {dummy_decompress};
    
    printf("Original data (%zu bytes): ", input_len);
    print_hex(input_data, input_len);
    
    Buffer compressed = process_data(input_data, input_len, compressors[0]);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompressors[0]);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_len && memcmp(input_data, decompressed.data, input_len) == 0) {
        printf("Round-trip successful!\n");
        printf("Decompressed text: %s\n", decompressed.data);
    } else {
        printf("Round-trip failed!\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}