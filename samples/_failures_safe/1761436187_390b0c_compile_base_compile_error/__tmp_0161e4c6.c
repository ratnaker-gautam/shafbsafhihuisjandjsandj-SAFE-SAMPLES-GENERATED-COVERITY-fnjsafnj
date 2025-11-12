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
        
        for (uint8_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

Buffer compress_null(Buffer input) {
    if (input.data == NULL || input.size == 0) {
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
    printf("Enter text to compress: ");
    
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
    Buffer compressed = compress(input);
    
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
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
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compressors[] = {compress_rle, compress_null};
    DecompressFunc decompressors[] = {decompress_rle, decompress_null};
    const char *names[] = {"RLE", "Null"};
    int num_methods = 2;
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_methods; i++) {
        printf("%d. %s\n", i + 1, names[i]);
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
    
    process_compression(compressors[choice - 1], decompressors[choice - 1]);
    
    return