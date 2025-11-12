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

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
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
    
    buf.data = malloc(len);
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
        free(input.data);
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
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
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    
    if (decompressed_size == input.size && memcmp(input.data, decompressed, input.size) == 0) {
        printf("Decompression verified: data matches original.\n");
    } else {
        printf("Decompression verification failed.\n");
    }
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Simple Run-Length Encoding Demo\n");
    printf("===============================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}