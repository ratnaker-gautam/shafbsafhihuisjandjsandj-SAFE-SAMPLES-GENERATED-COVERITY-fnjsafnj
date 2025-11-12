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

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = decompressed_size;
    return result;
}

int compare_buffers(const uint8_t *buf1, size_t size1, const uint8_t *buf2, size_t size2) {
    if (buf1 == NULL || buf2 == NULL || size1 != size2) {
        return 0;
    }
    
    for (size_t i = 0; i < size1; i++) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    CompressFunc compressors[] = {dummy_compress};
    DecompressFunc decompressors[] = {dummy_decompress};
    const char *test_strings[] = {"Hello, World!", "Test compression", "Another sample"};
    const int num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    
    for (int i = 0; i < num_tests; i++) {
        const char *test_str = test_strings[i];
        size_t input_len = strlen(test_str);
        
        if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
            continue;
        }
        
        Buffer compressed = compress_data((const uint8_t*)test_str, input_len, compressors[0]);
        if (compressed.data == NULL || compressed.size == 0) {
            printf("Compression failed for: %s\n", test_str);
            continue;
        }
        
        Buffer decompressed = decompress_data(compressed.data, compressed.size, decompressors[0]);
        if (decompressed.data == NULL || decompressed.size == 0) {
            printf("Decompression failed for: %s\n", test_str);
            free(compressed.data);
            continue;
        }
        
        int match = compare_buffers((const uint8_t*)test_str, input_len, decompressed.data, decompressed.size);
        printf("Test %d: Original='%s', Match=%s\n", i + 1, test_str, match ? "YES" : "NO");
        
        free(compressed.data);
        free(decompressed.data);
    }
    
    uint8_t user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)user_input, MAX_INPUT_SIZE, stdin) != NULL) {
        size_t input_len = strlen((char*)user_input);