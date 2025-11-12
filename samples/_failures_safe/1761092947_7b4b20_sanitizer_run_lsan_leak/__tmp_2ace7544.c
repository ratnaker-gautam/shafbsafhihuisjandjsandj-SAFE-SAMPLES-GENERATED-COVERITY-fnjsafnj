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
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

Buffer get_user_input() {
    Buffer input;
    input.data = malloc(MAX_INPUT_SIZE);
    if (input.data == NULL) {
        input.size = 0;
        return input;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
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

void process_compression() {
    Buffer input = get_user_input();
    if (input.data == NULL || input.size == 0) {
        printf("Invalid input\n");
        return;
    }
    
    CompressFunc compress = compress_rle;
    DecompressFunc decompress = decompress_rle;
    
    printf("Original: %s\n", input.data);
    printf("Original size: %zu bytes\n", input.size);
    
    Buffer compressed = compress(input);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        free(input.data);
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input.size * 100.0f);
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
    } else {
        printf("Decompressed: %s\n", decompressed.data);
        printf("Decompression successful: %s\n", 
               memcmp(input.data, decompressed.data, input.size) == 0 ? "Yes" : "No");
        free(decompressed.data);
    }
    
    free(input.data);
    free(compressed.data);
}

int main() {
    printf("RLE Compression Demo\n");
    printf("====================\n");
    
    process_compression();
    
    return 0;
}