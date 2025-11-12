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

Buffer create_buffer(size_t size) {
    Buffer buf = {NULL, 0};
    if (size > 0 && size <= MAX_OUTPUT_SIZE) {
        buf.data = malloc(size);
        if (buf.data != NULL) {
            buf.size = size;
        }
    }
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int get_user_input(uint8_t *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", buffer_size - 1);
    
    if (fgets((char*)buffer, buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? 1 : 0;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    uint8_t input[MAX_INPUT_SIZE] = {0};
    uint8_t compressed[MAX_OUTPUT_SIZE] = {0};
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    
    if (!get_user_input(input, sizeof(input))) {
        printf("Invalid input or empty string\n");
        return;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len == 0) {
        printf("Empty input string\n");
        return;
    }
    
    size_t compressed_size = compress(input, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != input_len) {
        printf("Decompression failed\n");
        return;
    }
    
    if (memcmp(input, decompressed, input_len) != 0) {
        printf("Data mismatch after decompression\n");
        return;
    }
    
    printf("Decompressed successfully: %s\n", decompressed);
}

int main() {
    CompressFunc compress_funcs[] = {dummy_compress};
    DecompressFunc decompress_funcs[] = {dummy_decompress};
    
    int choice = 0;
    printf("Compression Demo\n");
    printf("1. Process text\n");
    printf("2. Exit\n");
    printf("Choose option: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    getchar();
    
    switch (choice) {
        case 1:
            process_compression(compress_funcs[0], decompress_funcs[0]);
            break;
        case 2:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}