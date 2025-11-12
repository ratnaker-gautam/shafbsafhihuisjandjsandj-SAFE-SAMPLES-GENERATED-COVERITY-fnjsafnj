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

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        for (uint8_t i = 0; i < count && out_idx < output_size; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    char input[MAX_INPUT_SIZE];
    
    printf("Enter data to compress: ");
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
    
    buffer.data = (uint8_t *)malloc(len);
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
        printf("No input data received.\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input.data, input.size, compressed, sizeof(compressed));
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input.data);
        return;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input.size, compressed_size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    
    if (decompressed_size == 0) {
        printf("Decompression failed.\n");
        free(input.data);
        return;
    }
    
    if (decompressed_size == input.size && memcmp(input.data, decompressed, input.size) == 0) {
        printf("Round-trip verification successful.\n");
    } else {
        printf("Round-trip verification failed.\n");
    }
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = compress_rle;
    DecompressFunc decompress_func = decompress_rle;
    
    printf("RLE Compression Demo\n");
    process_compression(compress_func, decompress_func);
    
    return 0;
}