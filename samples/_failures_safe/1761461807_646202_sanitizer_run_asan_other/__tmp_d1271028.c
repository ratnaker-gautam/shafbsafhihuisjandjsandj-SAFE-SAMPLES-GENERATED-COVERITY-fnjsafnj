//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char* input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    size_t chunk_size = (end - start) / 2;
    if (chunk_size == 0) chunk_size = 1;
    
    if (chunk_size > 256) chunk_size = 256;
    
    Buffer left = compress_chunk(input, start, start + chunk_size);
    Buffer right = compress_chunk(input, start + chunk_size, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = left.size + right.size + 2;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data[0] = (unsigned char)chunk_size;
    memcpy(result.data + 1, left.data, left.size);
    result.data[1 + left.size] = (unsigned char)((end - start) - chunk_size);
    memcpy(result.data + 2 + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    return result;
}

void decompress_chunk(const unsigned char* compressed, size_t* offset, unsigned char* output, size_t* out_offset) {
    if (!compressed || !offset || !output || !out_offset) return;
    
    unsigned char left_size = compressed[*offset];
    (*offset)++;
    
    if (left_size == 0) return;
    
    unsigned char right_size = compressed[*offset + left_size];
    
    decompress_chunk(compressed, offset, output, out_offset);
    
    size_t copy_start = *offset;
    size_t copy_end = copy_start + left_size;
    
    if (copy_end > MAX_INPUT_SIZE || *out_offset + left_size > MAX_INPUT_SIZE) return;
    
    for (size_t i = copy_start; i < copy_end && i < MAX_INPUT_SIZE; i++) {
        output[*out_offset] = compressed[i];
        (*out_offset)++;
    }
    
    *offset += left_size + 1;
    
    decompress_chunk(compressed, offset, output, out_offset);
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char output[MAX_INPUT_SIZE] = {0};
    size_t input_len = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE - 1) {
        input[input_len++] = (unsigned char)c;
    }
    input[input_len] = '\0';
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, input_len);
    
    if (!compressed.data) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes (original: %zu bytes)\n", compressed.size, input_len);
    
    size_t decomp_offset = 0;
    size_t out_offset = 0;
    decompress_chunk(compressed.data, &decomp_offset, output, &out_offset);
    output[out_offset] = '\0';
    
    printf("Decompressed: %s\n", output);
    
    if (strcmp((char*)input, (char*)output) == 0) {
        printf("Compression and decompression successful.\n");
    } else {
        printf("Error: decompressed data doesn't match original.\n");
    }
    
    free(compressed.data);
    return 0;
}