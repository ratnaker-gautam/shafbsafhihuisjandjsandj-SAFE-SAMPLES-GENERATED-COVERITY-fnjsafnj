//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
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

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || compress_func == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t compressed_size = compress_func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output;
        result.size = compressed_size;
    } else {
        free(output);
    }
    
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || decompress_func == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t decompressed_size = decompress_func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output;
        result.size = decompressed_size;
    } else {
        free(output);
    }
    
    return result;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 2];
    
    printf("Enter data to compress (hex string, max %d bytes): ", MAX_INPUT_SIZE);
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
        printf("Empty input\n");
        return 1;
    }
    
    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        printf("Invalid hex string length\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    for (size_t i = 0; i < data_size; i++) {
        int high_nibble = hex_char_to_int(input_str[i * 2]);
        int low_nibble = hex_char_to_int(input_str[i