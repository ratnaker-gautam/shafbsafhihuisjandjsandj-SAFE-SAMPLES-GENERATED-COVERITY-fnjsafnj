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

typedef Buffer (*CompressFunc)(const Buffer*);
typedef Buffer (*DecompressFunc)(const Buffer*);

Buffer dummy_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return output;
    }
    
    output.size = input->size + 4;
    if (output.size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, "CMP", 3);
    output.data[3] = (uint8_t)(input->size & 0xFF);
    memcpy(output.data + 4, input->data, input->size);
    
    return output;
}

Buffer dummy_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size < 5 || input->size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    if (memcmp(input->data, "CMP", 3) != 0) {
        return output;
    }
    
    size_t orig_size = input->data[3];
    if (input->size != orig_size + 4) {
        return output;
    }
    
    output.size = orig_size;
    if (output.size > MAX_INPUT_SIZE) {
        return output;
    }
    
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input->data + 4, output.size);
    return output;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input = {(uint8_t*)input_str, len};
    Buffer compressed = compress(&input);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress(&compressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input.size && memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Decompression successful: %.*s\n", (int)decompressed.size, decompressed.data);
    } else {
        printf("Decompression mismatch\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}