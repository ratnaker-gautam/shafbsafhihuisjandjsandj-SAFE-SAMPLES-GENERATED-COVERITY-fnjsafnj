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

typedef Buffer (*CompressFunc)(const Buffer*);
typedef Buffer (*DecompressFunc)(const Buffer*);

Buffer dummy_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return output;
    }
    
    output.size = input->size * 2;
    output.data = malloc(output.size);
    if (!output.data) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i * 2] = input->data[i];
        output.data[i * 2 + 1] = 0xFF;
    }
    
    return output;
}

Buffer dummy_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0 || input->size > MAX_OUTPUT_SIZE || input->size % 2 != 0) {
        return output;
    }
    
    output.size = input->size / 2;
    output.data = malloc(output.size);
    if (!output.data) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0; i < output.size; i++) {
        if (input->data[i * 2 + 1] != 0xFF) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        output.data[i] = input->data[i * 2];
    }
    
    return output;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
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
    
    if (!compressed.data || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress(&compressed);
    
    if (!decompressed.data || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed text: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf("\n");
    
    int match = (decompressed.size == input.size && 
                memcmp(decompressed.data, input.data, input.size) == 0);
    printf("Round-trip successful: %s\n", match ? "Yes" : "No");
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Demo\n");
    printf("======================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}