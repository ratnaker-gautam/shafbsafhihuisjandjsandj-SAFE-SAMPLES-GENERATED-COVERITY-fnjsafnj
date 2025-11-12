//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef void (*CompressionFunc)(Buffer *input, Buffer *output);
typedef void (*DecompressionFunc)(Buffer *input, Buffer *output);

void dummy_compress(Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size && out_idx < COMPRESSED_SIZE - 2; i++) {
        output->data[out_idx++] = input->data[i];
        if (i > 0 && input->data[i] == input->data[i-1]) {
            uint8_t count = 2;
            while (i + 1 < input->size && input->data[i] == input->data[i+1] && count < 255) {
                count++;
                i++;
            }
            output->data[out_idx++] = count;
        }
    }
    output->size = out_idx;
}

void dummy_decompress(Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size && out_idx < MAX_INPUT_SIZE - 1; i++) {
        if (i + 1 < input->size && input->data[i+1] > 1 && input->data[i+1] < 255) {
            uint8_t count = input->data[i+1];
            for (uint8_t j = 0; j < count && out_idx < MAX_INPUT_SIZE - 1; j++) {
                output->data[out_idx++] = input->data[i];
            }
            i++;
        } else {
            output->data[out_idx++] = input->data[i];
        }
    }
    output->size = out_idx;
}

int get_user_input(char *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets(buffer, max_size, stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    return len > 0;
}

void process_compression(CompressionFunc compress, DecompressionFunc decompress) {
    if (compress == NULL || decompress == NULL) return;
    
    char input_text[MAX_INPUT_SIZE];
    if (!get_user_input(input_text, sizeof(input_text))) {
        printf("No input received.\n");
        return;
    }
    
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    original.data = (uint8_t*)malloc(MAX_INPUT_SIZE);
    compressed.data = (uint8_t*)malloc(COMPRESSED_SIZE);
    decompressed.data = (uint8_t*)malloc(MAX_INPUT_SIZE);
    
    if (original.data == NULL || compressed.data == NULL || decompressed.data == NULL) {
        printf("Memory allocation failed.\n");
        goto cleanup;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too long.\n");
        goto cleanup;
    }
    
    memcpy(original.data, input_text, input_len);
    original.size = input_len;
    compressed.size = 0;
    decompressed.size = 0;
    
    memset(compressed.data, 0, COMPRESSED_SIZE);
    memset(decompressed.data, 0, MAX_INPUT_SIZE);
    
    compress(&original, &compressed);
    decompress(&compressed, &decompressed);
    
    printf("Original size: %zu bytes\n", original.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Compression/decompression successful.\n");
    } else {
        printf("Compression/decompression failed.\n");
    }

cleanup:
    free(original.data);
    free(compressed.data);
    free(decompressed.data);
}

int main(void) {
    CompressionFunc compress_func = dummy_compress;
    DecompressionFunc decompress_func = dummy_decompress;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}