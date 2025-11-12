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

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

Buffer compress_data(const Buffer *input, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output_data);
        return result;
    }
    
    result.data = output_data;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const Buffer *input, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (!validate_buffer(input) || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output_data = malloc(MAX_OUTPUT_SIZE);
    if (output_data == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input->data, input->size, output_data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(output_data);
        return result;
    }
    
    result.data = output_data;
    result.size = decompressed_size;
    return result;
}

void print_buffer_hex(const Buffer *buf) {
    if (!validate_buffer(buf)) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02x", buf->data[i]);
    }
    printf("\n");
}

void cleanup_buffers(Buffer *buf1, Buffer *buf2, Buffer *buf3) {
    if (buf1 != NULL && buf1->data != NULL) {
        free(buf1->data);
        buf1->data = NULL;
    }
    if (buf2 != NULL && buf2->data != NULL) {
        free(buf2->data);
        buf2->data = NULL;
    }
    if (buf3 != NULL && buf3->data != NULL) {
        free(buf3->data);
        buf3->data = NULL;
    }
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    Buffer input_buf = {NULL, 0};
    Buffer compressed_buf = {NULL, 0};
    Buffer decompressed_buf = {NULL, 0};
    int result = 1;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    uint8_t *input_copy = malloc(input_len);
    if (input_copy == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    memcpy(input_copy, input_str, input_len);
    
    input_buf.data = input_copy;
    input_buf.size = input_len;
    
    compressed_buf = compress_data(&input_buf, dummy_compress);
    if (compressed_buf.data == NULL) {
        printf("Compression failed\n");
        cleanup_buffers(&input_buf, &compressed_buf, &decompressed_buf);
        return 1;
    }
    
    decompressed_buf