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
    memcpy(output, input, input_size);
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

int compress_data(const Buffer *input, Buffer *output, CompressFunc compress_fn) {
    if (!validate_buffer(input) || output == NULL || compress_fn == NULL) {
        return 0;
    }
    
    if (output->data == NULL) {
        output->data = malloc(MAX_OUTPUT_SIZE);
        if (output->data == NULL) {
            return 0;
        }
        output->size = MAX_OUTPUT_SIZE;
    }
    
    size_t result_size = compress_fn(input->data, input->size, output->data, output->size);
    if (result_size == 0 || result_size > output->size) {
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

int decompress_data(const Buffer *input, Buffer *output, DecompressFunc decompress_fn) {
    if (!validate_buffer(input) || output == NULL || decompress_fn == NULL) {
        return 0;
    }
    
    if (output->data == NULL) {
        output->data = malloc(MAX_OUTPUT_SIZE);
        if (output->data == NULL) {
            return 0;
        }
        output->size = MAX_OUTPUT_SIZE;
    }
    
    size_t result_size = decompress_fn(input->data, input->size, output->data, output->size);
    if (result_size == 0 || result_size > output->size) {
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    input_buf.data = (uint8_t*)user_input;
    input_buf.size = input_len;
    
    CompressFunc compress_fn = dummy_compress;
    DecompressFunc decompress_fn = dummy_decompress;
    
    printf("Original data (%zu bytes): ", input_buf.size);
    print_hex(input_buf.data, input_buf.size);
    
    if (!compress_data(&input_buf, &compressed_buf, compress_fn)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_buf.size);
    print_hex(compressed_buf.data, compressed_buf.size);
    
    if (!decompress_data(&compressed_buf, &decompressed_buf, decompress_fn)) {
        printf("Decompression failed\n");
        free(compressed_buf.data);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_buf.size);
    print_hex(decompressed_buf.data, decompressed_buf.size);
    
    if (decompressed_buf.size == input_buf.size && 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    free(compressed_buf.data);
    free(decompressed_buf.data);
    
    return 0;
}