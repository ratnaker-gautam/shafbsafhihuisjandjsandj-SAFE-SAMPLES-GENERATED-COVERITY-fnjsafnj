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

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 2 < output_size; i++) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < 255 && out_idx + 2 < output_size) {
            count++;
            i++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    char input[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return buffer;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return buffer;
    }
    
    buffer.data = (uint8_t*)malloc(len);
    if (buffer.data == NULL) {
        return buffer;
    }
    
    memcpy(buffer.data, input, len);
    buffer.size = len;
    return buffer;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input = read_input();
    if (input.data == NULL || input.size == 0) {
        printf("No valid input provided.\n");
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
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (input.size > 0) {
        double ratio = (double)compressed_size / input.size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != input.size) {
        printf("Decompression verification failed.\n");
        free(input.data);
        return;
    }
    
    if (memcmp(input.data, decompressed, input.size) == 0) {
        printf("Decompression verified successfully.\n");
    } else {
        printf("Decompression mismatch detected.\n");
    }
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = simple_compress;
    DecompressFunc decompress_func = simple_decompress;
    
    printf("Simple Run-Length Encoding Compression Demo\n");
    printf("==========================================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}