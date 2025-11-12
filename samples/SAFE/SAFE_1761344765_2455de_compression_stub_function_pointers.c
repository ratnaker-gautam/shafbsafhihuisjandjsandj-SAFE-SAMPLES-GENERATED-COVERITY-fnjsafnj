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
        if (i >= output_size) {
            return 0;
        }
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) {
            return 0;
        }
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
    
    size_t result_size = func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = result_size;
    return result;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    char user_input[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)user_input[i];
    }
    
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    Buffer compressed = process_data(input_data, input_len, compress_func);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    
    Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompress_func);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    if (decompressed.size != input_len) {
        printf("Size mismatch after decompression\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input_len; i++) {
        if (input_data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}