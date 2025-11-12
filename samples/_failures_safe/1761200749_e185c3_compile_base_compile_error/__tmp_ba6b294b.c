//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size);

size_t dummy_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size) return 0;
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) return 0;
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

size_t dummy_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size) return 0;
    
    for (size_t i = 0; i < input_size; i++) {
        if (i >= output_size) return 0;
        output[i] = input[i] ^ 0x55;
    }
    return input_size;
}

Buffer create_buffer(size_t size) {
    Buffer buf = {NULL, 0};
    if (size == 0 || size > 1024 * 1024) return buf;
    
    buf.data = malloc(size);
    if (buf.data != NULL) {
        buf.size = size;
    }
    return buf;
}

void free_buffer(Buffer* buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int compress_data(CompressFunc compress, const Buffer* input, Buffer* output) {
    if (compress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    
    size_t compressed_size = compress(input->data, input->size, output->data, output->size);
    if (compressed_size == 0 || compressed_size > output->size) return 0;
    
    output->size = compressed_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer* input, Buffer* output) {
    if (decompress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    
    size_t decompressed_size = decompress(input->data, input->size, output->data, output->size);
    if (decompressed_size == 0 || decompressed_size > output->size) return 0;
    
    output->size = decompressed_size;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->data == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        if (i > 0 && i % 16 == 0) printf("\n");
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    char input_str[256];
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
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
    
    Buffer input_buf = create_buffer(input_len);
    if (input_buf.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    memcpy(input_buf.data, input_str, input_len);
    
    Buffer compressed_buf = create_buffer(input_len * 2);
    if (compressed_buf.data == NULL) {
        printf("Memory allocation failed\n");
        free_buffer(&input_buf);
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_buf.size);
    print_buffer_hex(&input_buf);
    
    if (!compress_data(compress_func, &input_buf, &compressed_buf)) {
        printf("Compression failed\n");
        free_buffer(&input_buf);
        free_buffer(&compressed_buf);
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_buf.size);
    print_buffer_hex(&compressed_buf);
    
    Buffer decompressed_buf = create_buffer(input_len * 2);
    if (decompressed_buf.data == NULL) {
        printf("Memory allocation failed\n");
        free_buffer(&input_buf);
        free