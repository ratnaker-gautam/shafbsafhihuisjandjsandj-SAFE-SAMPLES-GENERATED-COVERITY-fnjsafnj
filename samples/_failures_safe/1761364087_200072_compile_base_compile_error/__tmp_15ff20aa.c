//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(const Buffer* input);
typedef Buffer (*DecompressionFunc)(const Buffer* input);

Buffer run_length_encode(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    uint8_t* compressed = malloc(MAX_OUTPUT_SIZE);
    if (compressed == NULL) {
        return output;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input->size && out_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_index + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        compressed[out_index++] = (uint8_t)count;
        compressed[out_index++] = current;
        i += count;
    }
    
    output.data = compressed;
    output.size = out_index;
    return output;
}

Buffer run_length_decode(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        return output;
    }
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (input->data[i] > MAX_OUTPUT_SIZE - estimated_size) {
            return output;
        }
        estimated_size += input->data[i];
        if (estimated_size > MAX_OUTPUT_SIZE) {
            return output;
        }
    }
    
    uint8_t* decompressed = malloc(estimated_size);
    if (decompressed == NULL) {
        return output;
    }
    
    size_t out_index = 0;
    for (size_t i = 0; i < input->size && out_index < estimated_size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count && out_index < estimated_size; j++) {
            decompressed[out_index++] = value;
        }
    }
    
    output.data = decompressed;
    output.size = out_index;
    return output;
}

void process_compression(CompressionFunc compress, DecompressionFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input = {(uint8_t*)input_str, len};
    Buffer compressed = compress(&input);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input.size * 100.0f);
    
    Buffer decompressed = decompress(&compressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compress_funcs[] = {run_length_encode};
    DecompressionFunc decompress_funcs[] = {run_length_decode};
    const char* method_names[] = {"Run-Length Encoding"};
    int num_methods = 1;
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_methods; i++) {
        printf("%d. %s\n", i + 1, method_names[i]);
    }
    
    printf("Select method (1-%d): ", num_methods);
    
    char choice_str[10];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Error reading choice\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    if (choice < 1 || choice > num_methods) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_compression(