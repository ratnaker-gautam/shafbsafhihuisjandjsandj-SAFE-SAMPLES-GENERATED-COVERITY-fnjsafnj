//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(const Buffer*);

Buffer compress_rle(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    output.data = malloc(input->size * COMPRESSED_MULTIPLIER);
    if (!output.data) return output;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 > input->size * COMPRESSED_MULTIPLIER) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        output.data[out_idx++] = (uint8_t)count;
        output.data[out_idx++] = current;
        in_idx += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer compress_copy(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    output.data = malloc(input->size);
    if (!output.data) return output;
    
    memcpy(output.data, input->data, input->size);
    output.size = input->size;
    return output;
}

Buffer decompress_rle(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0 || input->size % 2 != 0) return output;
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        estimated_size += input->data[i];
        if (estimated_size > MAX_INPUT_SIZE) {
            return output;
        }
    }
    
    output.data = malloc(estimated_size);
    if (!output.data) return output;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > estimated_size) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

void process_compression(CompressionFunc compress, CompressionFunc decompress, const char *name) {
    printf("Testing %s compression\n", name);
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    char input_text[MAX_INPUT_SIZE + 1];
    if (!fgets(input_text, sizeof(input_text), stdin)) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[--len] = '\0';
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input = {(uint8_t*)input_text, len};
    Buffer compressed = compress(&input);
    
    if (!compressed.data) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size, compressed.size);
    
    if (decompress) {
        Buffer decompressed = decompress(&compressed);
        if (!decompressed.data) {
            printf("Decompression failed\n");
            free(compressed.data);
            return;
        }
        
        printf("Decompressed matches original: %s\n", 
               (decompressed.size == input.size && 
                memcmp(decompressed.data, input.data, input.size) == 0) ? "yes" : "no");
        
        free(decompressed.data);
    }
    
    free(compressed.data);
}

int main() {
    CompressionFunc compressors[] = {compress_rle, compress_copy};
    CompressionFunc decompressors[] = {decompress_rle, NULL};
    const char *names[] = {"RLE", "Copy"};
    
    printf("Compression Algorithm Test\n");
    printf("==========================\n");
    
    for (size_t i = 0; i < sizeof(compressors) / sizeof(compressors[0]); i++) {
        process_compression(compressors[i], decompressors[i], names[i]);
        printf("\n");
    }
    
    return 0;
}