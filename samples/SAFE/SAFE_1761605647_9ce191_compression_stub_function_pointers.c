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

typedef Buffer (*CompressFunc)(Buffer);
typedef Buffer (*DecompressFunc)(Buffer);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0) {
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
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
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
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[in_pos++];
        uint8_t value = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
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
    CompressFunc compress = compress_rle;
    DecompressFunc decompress = decompress_rle;
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char input_line[MAX_INPUT_SIZE * 3];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
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
    char *token = strtok(input_line, " \n");
    
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) == 1 && byte <= 0xFF) {
            input.data[input.size++] = (uint8_t)byte;
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
    
    Buffer compressed = compress(input);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        free(input.data);
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer(compressed);
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(input.data);
        free(compressed.data);
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer(decompressed);
    
    if (input.size == decompressed.size && 
        memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("\nCompression/decompression successful!\n");
    } else {
        printf("\nCompression/decompression failed - data mismatch!\n");
    }
    
    free(input.data);
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}