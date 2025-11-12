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

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    char input[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return buf;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return buf;
    }
    
    buf.data = (uint8_t*)malloc(len);
    if (buf.data == NULL) {
        return buf;
    }
    
    memcpy(buf.data, input, len);
    buf.size = len;
    return buf;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input = read_input();
    if (input.data == NULL || input.size == 0) {
        printf("No input data received.\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    size_t compressed_size = compress(input.data, input.size, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input.data);
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size, compressed_size);
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0) {
        printf("Decompression failed.\n");
        free(input.data);
        return;
    }
    
    if (decompressed_size != input.size || memcmp(input.data, decompressed, input.size) != 0) {
        printf("Verification failed: decompressed data doesn't match original.\n");
    } else {
        printf("Verification successful: data compressed and decompressed correctly.\n");
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size && i < 50; i++) {
        printf("%c", decompressed[i]);
    }
    if (decompressed_size > 50) printf("...");
    printf("\n");
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = compress_rle;
    DecompressFunc decompress_func = decompress_rle;
    
    printf("RLE Compression Demo\n");
    printf("====================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}