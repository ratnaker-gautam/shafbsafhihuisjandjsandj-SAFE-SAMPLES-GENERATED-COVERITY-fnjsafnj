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
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input.size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input.size && 
               input.data[in_idx + count] == current && 
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
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input.size && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[in_idx++];
        uint8_t value = input.data[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

Buffer compress_null(Buffer input) {
    if (input.data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(input.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data, input.size);
    output.size = input.size;
    return output;
}

Buffer decompress_null(Buffer input) {
    return compress_null(input);
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
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
    Buffer compressed = compress(input);
    
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        if (compressed.data) free(compressed.data);
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf("\n");
    
    if (memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compressors[] = {compress_rle, compress_null};
    DecompressFunc decompressors[] = {decompress_rle, decompress_null};
    const char *names[] = {"RLE Compression", "Null Compression"};
    size_t num_methods = sizeof(names) / sizeof(names[0]);
    
    printf("Available compression methods:\n");
    for (size_t i = 0; i < num_methods; i++) {
        printf("%zu. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-%zu): ", num_methods);
    
    char