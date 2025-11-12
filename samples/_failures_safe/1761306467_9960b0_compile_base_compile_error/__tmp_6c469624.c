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

typedef Buffer (*CompressionFunc)(Buffer input);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size > MAX_INPUT_SIZE) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input.size && 
               input.data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer decompress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size > MAX_INPUT_SIZE) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos + 1 < input.size && out_pos < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[in_pos++];
        uint8_t value = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) {
            count = MAX_OUTPUT_SIZE - out_pos;
        }
        
        for (uint8_t i = 0; i < count && out_pos < MAX_OUTPUT_SIZE; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

Buffer process_data(Buffer input, CompressionFunc func) {
    if (func == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    return func(input);
}

void print_buffer(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf.size % 16 != 0) printf("\n");
}

int main() {
    printf("Compression Stub Demo\n");
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    Buffer input;
    input.data = malloc(MAX_INPUT_SIZE);
    if (input.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    input.size = 0;
    char *token = strtok(line, " \n");
    
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) == 1) {
            input.data[input.size++] = (uint8_t)value;
        }
        token = strtok(NULL, " \n");
    }
    
    if (input.size == 0) {
        printf("No valid input data\n");
        free(input.data);
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer(input);
    
    CompressionFunc compressors[] = {compress_rle, NULL};
    CompressionFunc decompressors[] = {decompress_rle, NULL};
    
    for (int i = 0; compressors[i] != NULL; i++) {
        printf("\nCompressing with algorithm %d:\n", i + 1);
        
        Buffer compressed = process_data(input, compressors[i]);
        if (compressed.data == NULL || compressed.size == 0) {
            printf("Compression failed\n");
            continue;
        }
        
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer(compressed);
        
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / input.size * 100);
        
        Buffer decompressed = process_data(compressed, decompressors[i]);
        if (decompressed.data == NULL || decompressed.size == 0) {
            printf("Decompression failed\n");
            free(compressed.data);
            continue;
        }
        
        printf("Decompressed data (%zu bytes):\n", decompressed.size);
        print_buffer