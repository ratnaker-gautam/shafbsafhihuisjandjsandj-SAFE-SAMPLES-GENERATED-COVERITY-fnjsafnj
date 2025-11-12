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

typedef Buffer (*CompressionFunc)(Buffer);
typedef Buffer (*DecompressionFunc)(Buffer);

Buffer dummy_compress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size + 4;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    output.data[0] = 'C';
    output.data[1] = 'M';
    output.data[2] = 'P';
    output.data[3] = 'R';
    memcpy(output.data + 4, input.data, input.size);
    
    return output;
}

Buffer dummy_decompress(Buffer input) {
    Buffer output;
    if (input.size < 4 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    if (input.data[0] != 'C' || input.data[1] != 'M' || 
        input.data[2] != 'P' || input.data[3] != 'R') {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size - 4;
    if (output.size == 0) {
        output.data = NULL;
        return output;
    }
    
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data + 4, output.size);
    return output;
}

void process_compression(CompressionFunc compress, DecompressionFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    Buffer input, compressed, decompressed;
    
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
    
    input.size = len;
    input.data = (uint8_t*)input_str;
    
    compressed = compress(input);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           ((float)compressed.size / (float)input.size) * 100.0f);
    
    decompressed = decompress(compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Decompression successful: %.*s\n", (int)decompressed.size, decompressed.data);
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compress_func = dummy_compress;
    DecompressionFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Demo\n");
    printf("======================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}