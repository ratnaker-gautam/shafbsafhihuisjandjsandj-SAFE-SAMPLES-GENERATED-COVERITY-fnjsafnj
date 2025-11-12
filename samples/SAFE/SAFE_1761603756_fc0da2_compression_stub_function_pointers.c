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
        output[i] = input[i] + 1;
    }
    
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] - 1;
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
    Buffer input_buffer = read_input();
    if (input_buffer.data == NULL || input_buffer.size == 0) {
        printf("No valid input received.\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    size_t compressed_size = compress(input_buffer.data, input_buffer.size, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input_buffer.data);
        return;
    }
    
    printf("Original size: %zu bytes\n", input_buffer.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != input_buffer.size) {
        printf("Decompression failed.\n");
        free(input_buffer.data);
        return;
    }
    
    if (memcmp(input_buffer.data, decompressed, input_buffer.size) != 0) {
        printf("Data verification failed.\n");
    } else {
        printf("Data verified successfully.\n");
    }
    
    printf("Original: ");
    fwrite(input_buffer.data, 1, input_buffer.size, stdout);
    printf("\n");
    
    printf("Decompressed: ");
    fwrite(decompressed, 1, decompressed_size, stdout);
    printf("\n");
    
    free(input_buffer.data);
}

int main(void) {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Test\n");
    printf("======================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}