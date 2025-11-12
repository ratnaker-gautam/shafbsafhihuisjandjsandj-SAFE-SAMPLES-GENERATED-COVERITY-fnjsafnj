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
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const char *name) {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Using compression: %s\n", name);
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input_data[MAX_INPUT_SIZE];
    if (fgets(input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    input_buf.data = (uint8_t*)input_data;
    input_buf.size = input_len;
    
    if (!validate_buffer(&input_buf)) {
        printf("Invalid input buffer\n");
        return;
    }
    
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    compressed_buf.data = compressed_data;
    compressed_buf.size = sizeof(compressed_data);
    
    size_t compressed_size = compress(input_buf.data, input_buf.size, compressed_buf.data, compressed_buf.size);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    compressed_buf.size = compressed_size;
    
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    decompressed_buf.data = decompressed_data;
    decompressed_buf.size = sizeof(decompressed_data);
    
    size_t decompressed_size = decompress(compressed_buf.data, compressed_buf.size, decompressed_buf.data, decompressed_buf.size);
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return;
    }
    
    decompressed_buf.size = decompressed_size;
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    
    if (input_buf.size > 0) {
        double ratio = (1.0 -