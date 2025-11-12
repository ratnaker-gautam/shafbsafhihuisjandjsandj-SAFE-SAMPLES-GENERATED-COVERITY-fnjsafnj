//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t dummy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
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
    
    buffer.data = malloc(len);
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
        printf("No valid input received.\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    size_t compressed_size = compress(input.data, input.size, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input.data);
        return;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (input.size > 0) {
        double ratio = ((double)compressed_size / input.size) * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != input.size) {
        printf("Decompression verification failed.\n");
        free(input.data);
        return;
    }
    
    if (memcmp(input.data, decompressed, input.size) != 0) {
        printf("Data integrity check failed.\n");
    } else {
        printf("Data integrity verified successfully.\n");
    }
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed_size && i < 100; i++) {
        if (decompressed[i] >= 32 && decompressed[i] <= 126) {
            putchar(decompressed[i]);
        } else {
            putchar('.');
        }
    }
    printf("\n");
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Demo\n");
    printf("======================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}