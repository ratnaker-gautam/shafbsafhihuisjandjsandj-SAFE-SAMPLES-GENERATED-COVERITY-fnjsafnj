//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

Buffer get_user_input() {
    Buffer input;
    input.data = malloc(MAX_INPUT_SIZE);
    if (input.data == NULL) {
        input.size = 0;
        return input;
    }
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input.data, MAX_INPUT_SIZE, stdin) == NULL) {
        free(input.data);
        input.data = NULL;
        input.size = 0;
        return input;
    }
    
    input.size = strlen((char*)input.data);
    if (input.size > 0 && input.data[input.size - 1] == '\n') {
        input.data[input.size - 1] = '\0';
        input.size--;
    }
    
    return input;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer original = get_user_input();
    if (original.data == NULL || original.size == 0) {
        printf("Error: Invalid input\n");
        return;
    }
    
    printf("Original: %s\n", original.data);
    printf("Original size: %zu bytes\n", original.size);
    
    Buffer compressed = compress(original);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Error: Compression failed\n");
        free(original.data);
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / original.size * 100.0f);
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Error: Decompression failed\n");
    } else {
        printf("Decompressed: %s\n", decompressed.data);
        printf("Decompression successful: %s\n", 
               (decompressed.size == original.size && 
                memcmp(decompressed.data, original.data, original.size) == 0) ? 
               "Yes" : "No");
        free(decompressed.data);
    }
    
    free(original.data);
    free(compressed.data);
}

int main() {
    CompressFunc compress_func = compress_rle;
    DecompressFunc decompress_func = decompress_rle;
    
    printf("RLE Compression Demo\n");
    printf("===================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}