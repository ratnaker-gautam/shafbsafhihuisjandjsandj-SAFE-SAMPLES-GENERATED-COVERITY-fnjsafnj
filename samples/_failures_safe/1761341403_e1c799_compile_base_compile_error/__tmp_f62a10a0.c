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

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

int compress_data(const Buffer *input, Buffer *output, CompressFunc compress) {
    if (!validate_buffer(input) || output == NULL || compress == NULL) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    return output->size > 0;
}

int decompress_data(const Buffer *input, Buffer *output, DecompressFunc decompress) {
    if (!validate_buffer(input) || output == NULL || decompress == NULL) return 0;
    
    output->data = malloc(MAX_INPUT_SIZE);
    if (output->data == NULL) return 0;
    
    output->size = decompress(input->data, input->size, output->data, MAX_INPUT_SIZE);
    return output->size > 0;
}

void print_buffer_hex(const Buffer *buf) {
    if (!validate_buffer(buf)) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

void cleanup_buffers(Buffer *b1, Buffer *b2, Buffer *b3) {
    if (b1 != NULL && b1->data != NULL) free(b1->data);
    if (b2 != NULL && b2->data != NULL) free(b2->data);
    if (b3 != NULL && b3->data != NULL) free(b3->data);
}

int main() {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    original.data = (uint8_t*)malloc(len);
    if (original.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    memcpy(original.data, input_str, len);
    original.size = len;
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("\nOriginal data (%zu bytes):\n", original.size);
    print_buffer_hex(&original);
    
    if (compress_data(&original, &compressed, compress_func)) {
        printf("\nCompressed data (%zu bytes):\n", compressed.size);
        print_buffer_hex(&compressed);
        
        float ratio = compressed.size * 100.0f / original.size;
        printf("Compression ratio: