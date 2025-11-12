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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (out_idx + 2 >= output_size) return 0;
        output[out_idx++] = input[i];
        output[out_idx++] = input[i];
    }
    return out_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    if (output_size < input_size / 2) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (out_idx >= output_size) return 0;
        if (i + 1 >= input_size) return 0;
        if (input[i] != input[i + 1]) return 0;
        output[out_idx++] = input[i];
    }
    return out_idx;
}

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    char input[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
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
        printf("No valid input received\n");
        free(input.data);
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE] = {0};
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    
    size_t compressed_size = compress(input.data, input.size, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        free(input.data);
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size, compressed_size);
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        free(input.data);
        return;
    }
    
    if (decompressed_size != input.size || memcmp(input.data, decompressed, input.size) != 0) {
        printf("Verification failed\n");
    } else {
        printf("Verification successful\n");
        printf("Decompressed text: ");
        fwrite(decompressed, 1, decompressed_size, stdout);
        printf("\n");
    }
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = simple_compress;
    DecompressFunc decompress_func = simple_decompress;
    
    printf("Simple Compression Demo\n");
    printf("======================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}