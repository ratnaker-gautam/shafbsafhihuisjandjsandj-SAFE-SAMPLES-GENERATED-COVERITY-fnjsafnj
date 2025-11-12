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

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func, size_t max_output_size) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || func == NULL || input_size == 0 || max_output_size == 0) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(max_output_size);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output_buffer, max_output_size);
    if (output_size == 0 || output_size > max_output_size) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = output_size;
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
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
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Original data (%zu bytes): ", input_len);
    print_buffer_hex(input_data, input_len);
    
    Buffer compressed = process_data(input_data, input_len, compress_func, MAX_OUTPUT_SIZE);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompress_func, MAX_INPUT_SIZE);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_len && memcmp(input_data, decompressed.data, input_len) == 0) {
        printf("Round-trip successful!\n");
    } else {