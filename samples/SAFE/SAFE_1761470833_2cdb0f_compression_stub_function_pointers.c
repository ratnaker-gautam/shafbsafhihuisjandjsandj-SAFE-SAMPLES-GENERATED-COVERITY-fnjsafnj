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
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = count;
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
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    size_t compressed_size = compress(input_buffer, input_len, compressed_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    
    size_t decompressed_size = decompress(compressed_buffer, compressed_size, decompressed_buffer, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return;
    }
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Decompression verification failed\n");
        return;
    }
    
    printf("Decompression successful - data verified\n");
    printf("Original: %s\n", input_buffer);
    printf("Decompressed: %s\n", decompressed_buffer);
}

int main() {
    CompressFunc compress_funcs[] = {run_length_encode};
    DecompressFunc decompress_funcs[] = {run_length_decode};
    const char* algorithm_names[] = {"Run-Length Encoding"};
    int num_algorithms = sizeof(compress_funcs) / sizeof(compress_funcs[0]);
    
    printf("Available compression algorithms:\n");
    for (int i = 0; i < num_algorithms; i++) {
        printf("%d. %s\n", i + 1, algorithm_names[i]);
    }
    
    printf("Select algorithm (1-%d): ", num_algorithms);
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Error reading choice\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    if (choice < 1 || choice > num_algorithms) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_compression(compress_funcs[choice - 1], decompress_funcs[choice - 1]);
    
    return 0;
}