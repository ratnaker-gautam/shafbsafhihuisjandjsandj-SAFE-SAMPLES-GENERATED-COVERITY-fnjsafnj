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
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    if (input_size > output_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

void print_buffer(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    CompressFunc compressors[] = {run_length_encode, null_compress};
    DecompressFunc decompressors[] = {run_length_decode, null_decompress};
    const char *names[] = {"Run-Length Encoding", "Null (Copy)"};
    const int num_methods = 2;
    
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter input data (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = 0;
    char *token = strtok(line, " \t\n");
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) == 1) {
            if (value <= 0xFF) {
                input[input_size++] = (uint8_t)value;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_buffer(input, input_size);
    
    for (int i = 0; i < num_methods; i++) {
        printf("\n--- %s ---\n", names[i]);
        
        size_t compressed_size = compressors[i](input, input_size, compressed, MAX_OUTPUT_SIZE);
        if (compressed_size == 0) {
            printf("Compression failed\n");
            continue;
        }
        
        printf("Compressed (%zu bytes, ratio: %.2f):\n", 
               compressed_size, (float)compressed_size / input_size);
        print_buffer(compressed, compressed_size);
        
        size_t decompressed_size = decompressors[i](compressed, compressed_size, 
                                                   decompressed, MAX_INPUT_SIZE);
        if (decompressed_size == 0) {
            printf("Decompression failed\n");
            continue;
        }
        
        printf("Decompressed (%zu bytes):\n", decompressed_size);
        print_buffer(decompressed, decompressed_size);