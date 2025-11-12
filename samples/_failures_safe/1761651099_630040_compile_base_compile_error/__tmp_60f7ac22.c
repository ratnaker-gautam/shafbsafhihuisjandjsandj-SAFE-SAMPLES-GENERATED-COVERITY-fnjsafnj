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
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
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
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return;
    }
    
    input.data = (uint8_t*)user_input;
    input.size = input_len;
    
    if (!validate_buffer(&input)) {
        printf("Invalid input buffer\n");
        return;
    }
    
    uint8_t comp_buf[MAX_OUTPUT_SIZE];
    compressed.data = comp_buf;
    compressed.size = sizeof(comp_buf);
    
    size_t comp_size = compress(input.data, input.size, compressed.data, compressed.size);
    if (comp_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    compressed.size = comp_size;
    
    uint8_t decomp_buf[MAX_INPUT_SIZE];
    decompressed.data = decomp_buf;
    decompressed.size = sizeof(decomp_buf);
    
    size_t decomp_size = decompress(compressed.data, compressed.size, decompressed.data, decompressed.size);
    if (decomp_size == 0) {
        printf("Decompression failed\n");
        return;
    }
    
    decompressed.size = decomp_size;
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input.size) * 100);
    
    if (input.size == decompressed.size && memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
}

int main() {
    CompressFunc compressors[] = {run_length_encode};
    DecompressFunc decompressors[] = {run_length_decode};
    const char* names[] = {"Run-Length Encoding"};
    
    int num_methods = sizeof(compressors) / sizeof(compressors[0]);
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_methods; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-%d): ", num_methods);
    
    char choice_str[16];
    if (fgets(