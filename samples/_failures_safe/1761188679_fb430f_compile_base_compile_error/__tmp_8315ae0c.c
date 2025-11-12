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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
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

size_t copy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t copy_size = (input_size < output_size) ? input_size : output_size;
    memcpy(output, input, copy_size);
    return copy_size;
}

size_t copy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return copy_compress(input, input_size, output, output_size);
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input data (max %d bytes): ", MAX_INPUT_SIZE);
    char input_str[MAX_INPUT_SIZE + 1];
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    input.data = (uint8_t*)input_str;
    input.size = input_len;
    
    if (!validate_buffer(&input)) {
        printf("Invalid input buffer\n");
        return;
    }
    
    uint8_t comp_buf[MAX_OUTPUT_SIZE];
    size_t comp_size = compress(input.data, input.size, comp_buf, sizeof(comp_buf));
    
    if (comp_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    uint8_t decomp_buf[MAX_OUTPUT_SIZE];
    size_t decomp_size = decompress(comp_buf, comp_size, decomp_buf, sizeof(decomp_buf));
    
    if (decomp_size == 0 || decomp_size != input.size) {
        printf("Decompression failed\n");
        return;
    }
    
    if (memcmp(input.data, decomp_buf, input.size) != 0) {
        printf("Data mismatch after compression/decompression\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", comp_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)comp_size / input.size) * 100);
    printf("Round-trip successful\n");
}

int main() {
    CompressFunc compressors[] = {run_length_encode, copy_compress};
    DecompressFunc decompressors[] = {run_length_decode, copy_decompress};
    const char *names[] = {"Run-Length Encoding", "Copy (no compression)"};
    size_t num_methods = sizeof(names) / sizeof(names[0]);
    
    printf("Available compression methods:\n");
    for (size_t i = 0; i < num_methods; i++) {
        printf("%zu. %s\n", i + 1, names[i]);