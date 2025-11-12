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

Buffer compress_null(Buffer input) {
    Buffer output;
    output.size = input.size;
    output.data = malloc(input.size);
    
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    if (input.data != NULL && input.size > 0) {
        memcpy(output.data, input.data, input.size);
    }
    
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
        
        for (uint8_t i = 0; i < count && out_pos < MAX_OUTPUT_SIZE; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

void process_compression(CompressionFunc compress, CompressionFunc decompress) {
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
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Original size: %zu bytes\n", input.size);
    
    if (compressed.size < input.size) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / input.size) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data != NULL && decompressed.size == input.size &&
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    printf("Compression Test Program\n");
    printf("1. RLE Compression\n");
    printf("2. Null Compression (copy)\n");
    
    char choice[10];
    printf("Select method (1-2): ");
    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        return 1;
    }
    
    int method = atoi(choice);
    
    switch (method) {
        case 1:
            process_compression(compress_rle, decompress_rle);
            break;
        case 2:
            process_compression(compress_null, compress_null);
            break;
        default:
            printf("Invalid selection\n");
            return 1;
    }
    
    return 0;
}