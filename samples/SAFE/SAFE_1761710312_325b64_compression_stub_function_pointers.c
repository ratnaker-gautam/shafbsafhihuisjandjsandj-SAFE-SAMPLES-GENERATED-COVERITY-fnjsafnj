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

typedef int (*CompressFunc)(const Buffer *input, Buffer *output);
typedef int (*DecompressFunc)(const Buffer *input, Buffer *output);

int dummy_compress(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) {
        return -1;
    }
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (output->size < input->size + 1) {
        return -1;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        output->data[i] = input->data[i] + 1;
    }
    output->size = input->size;
    return 0;
}

int dummy_decompress(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) {
        return -1;
    }
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (output->size < input->size) {
        return -1;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        output->data[i] = input->data[i] - 1;
    }
    output->size = input->size;
    return 0;
}

int process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return -1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)input_str[i];
    }
    
    Buffer input_buf = {input_data, input_len};
    Buffer compressed_buf = {compressed_data, sizeof(compressed_data)};
    Buffer decompressed_buf = {decompressed_data, sizeof(decompressed_data)};
    
    if (compress(&input_buf, &compressed_buf) != 0) {
        printf("Compression failed.\n");
        return -1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    
    if (decompress(&compressed_buf, &decompressed_buf) != 0) {
        printf("Decompression failed.\n");
        return -1;
    }
    
    if (decompressed_buf.size != input_len) {
        printf("Size mismatch after decompression.\n");
        return -1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (decompressed_data[i] != input_data[i]) {
            printf("Data corruption detected.\n");
            return -1;
        }
    }
    
    printf("Decompressed successfully: ");
    for (size_t i = 0; i < decompressed_buf.size; i++) {
        putchar(decompressed_data[i]);
    }
    printf("\n");
    
    return 0;
}

int main(void) {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    if (process_compression(compress_func, decompress_func) != 0) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}