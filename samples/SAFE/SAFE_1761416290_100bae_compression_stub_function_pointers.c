//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressFunc)(const Buffer*);
typedef Buffer (*DecompressFunc)(const Buffer*);

Buffer null_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    output.size = input->size + 1;
    output.data = malloc(output.size);
    if (output.data == NULL) return output;
    
    memcpy(output.data, input->data, input->size);
    output.data[input->size] = 0xFF;
    return output;
}

Buffer null_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) return output;
    
    output.size = input->size - 1;
    if (output.size == 0) return output;
    
    output.data = malloc(output.size);
    if (output.data == NULL) return output;
    
    memcpy(output.data, input->data, output.size);
    return output;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= 1024 * 1024;
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    CompressFunc compress = null_compress;
    DecompressFunc decompress = null_decompress;
    
    uint8_t test_data[] = "Hello, this is a test string for compression stub.";
    Buffer original = {test_data, sizeof(test_data) - 1};
    
    if (!validate_buffer(&original)) {
        fprintf(stderr, "Invalid input buffer\n");
        return 1;
    }
    
    printf("Original size: %zu\n", original.size);
    
    Buffer compressed = compress(&original);
    if (!validate_buffer(&compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    
    Buffer decompressed = decompress(&compressed);
    if (!validate_buffer(&decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        cleanup_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed size: %zu\n", decompressed.size);
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    cleanup_buffer(&compressed);
    cleanup_buffer(&decompressed);
    
    return 0;
}