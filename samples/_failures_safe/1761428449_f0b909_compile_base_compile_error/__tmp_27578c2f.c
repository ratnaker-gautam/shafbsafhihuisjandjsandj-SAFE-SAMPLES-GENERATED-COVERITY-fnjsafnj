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

typedef Buffer (*CompressionFunc)(const Buffer*);

Buffer compress_rle(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        return output;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_idx++] = (uint8_t)count;
        output.data[out_idx++] = current;
        in_idx += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer compress_null(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    if (input->size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    output.data = malloc(input->size);
    if (output.data == NULL) {
        return output;
    }
    
    memcpy(output.data, input->data, input->size);
    output.size = input->size;
    return output;
}

Buffer decompress_rle(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        return output;
    }
    
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        return output;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

void process_compression(CompressionFunc compress_func, CompressionFunc decompress_func) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress: ");
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
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
    Buffer compressed = compress_func(&input);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_func(&compressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf("\n");
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compressors[] = {compress_rle, compress_null};
    CompressionFunc decompressors[] = {decompress_rle, compress_null};
    const char *names[] = {"RLE Compression", "Null Compression"};
    
    printf("Available compression methods:\n");
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        printf("%zu. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-%zu): ", sizeof(names) / sizeof(names[0]));
    
    char choice_str[10];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    long choice = strtol(choice_str, NULL, 10);
    if (choice < 1 || choice > (long)(sizeof(names) / sizeof(names[0]))) {
        printf("Invalid choice\n");
        return