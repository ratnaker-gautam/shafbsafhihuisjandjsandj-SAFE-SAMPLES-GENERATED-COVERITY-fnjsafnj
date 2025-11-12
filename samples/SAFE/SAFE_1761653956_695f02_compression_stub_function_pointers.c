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
    
    memcpy(output.data, "CMP", 3);
    output.data[3] = (uint8_t)(input.size & 0xFF);
    memcpy(output.data + 4, input.data, input.size);
    
    return output;
}

Buffer dummy_decompress(Buffer input) {
    Buffer output;
    if (input.size < 4 || input.data == NULL || memcmp(input.data, "CMP", 3) != 0) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    size_t orig_size = input.data[3];
    if (input.size != orig_size + 4) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = orig_size;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data + 4, output.size);
    return output;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
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
    
    Buffer input;
    input.data = (uint8_t*)input_str;
    input.size = len;
    
    Buffer compressed = compress(input);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           ((float)compressed.size / (float)input.size) * 100.0f);
    
    Buffer decompressed = decompress(compressed);
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
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Stub Demo\n");
    printf("============================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}