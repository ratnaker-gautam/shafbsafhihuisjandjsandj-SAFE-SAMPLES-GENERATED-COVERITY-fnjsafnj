//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || func == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t output_size = func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (output_size > 0) {
        result.data = output;
        result.size = output_size;
    } else {
        free(output);
    }
    
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter input data as hex string (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        printf("Invalid hex string length\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};
        for (int j = 0; j < 2; j++) {
            if (!isxdigit((unsigned char)hex_byte[j])) {
                printf("Invalid hex character at position %zu\n", i * 2 + j);
                return 1;
            }
        }
        char *endptr;
        unsigned long value = strtoul(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value > 255) {
            printf("Invalid hex byte at position %zu\n", i);
            return 1;
        }
        input_buffer[i] = (uint8_t)value;
    }
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("\nOriginal data (%zu bytes):\n", data_size);
    print_buffer_hex(input_buffer, data_size);
    
    Buffer compressed = process_data(input_buffer, data_size, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu