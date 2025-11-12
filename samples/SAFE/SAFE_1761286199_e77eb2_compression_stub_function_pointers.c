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

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
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
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size, compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input.size - compressed.size) / input.size * 100);
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data == NULL || decompressed.size != input.size || 
        memcmp(decompressed.data, input.data, input.size) != 0) {
        printf("Decompression verification failed\n");
    } else {
        printf("Decompression successful\n");
        printf("Original: %.*s\n", (int)input.size, input.data);
        printf("Decompressed: %.*s\n", (int)decompressed.size, decompressed.data);
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compress_functions[] = {compress_rle};
    DecompressFunc decompress_functions[] = {decompress_rle};
    
    int choice = 0;
    printf("Compression Algorithm:\n");
    printf("1. Run-Length Encoding\n");
    printf("Select algorithm (1): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_compression(compress_functions[choice - 1], 
                       decompress_functions[choice - 1]);
    
    return 0;
}