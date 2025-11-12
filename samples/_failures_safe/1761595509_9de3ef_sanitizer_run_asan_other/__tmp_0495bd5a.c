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
    
    result.size = left.size + right.size + 3;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data[0] = (unsigned char)chunk_size;
    result.data[1] = (unsigned char)left.size;
    result.data[2] = (unsigned char)right.size;
    memcpy(result.data + 3, left.data, left.size);
    memcpy(result.data + 3 + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    return result;
}

void decompress_chunk(const unsigned char* compressed, size_t* offset, unsigned char* output, size_t* out_offset) {
    if (*offset >= MAX_INPUT_SIZE) return;
    
    unsigned char chunk_size = compressed[(*offset)++];
    unsigned char left_size = compressed[(*offset)++];
    unsigned char right_size = compressed[(*offset)++];
    
    if (chunk_size == 0) return;
    
    if (left_size > 0) {
        decompress_chunk(compressed, offset, output, out_offset);
    }
    
    size_t copy_end = *out_offset + chunk_size;
    if (copy_end > MAX_INPUT_SIZE) copy_end = MAX_INPUT_SIZE;
    
    for (size_t i = *out_offset; i < copy_end && i < MAX_INPUT_SIZE; i++) {
        output[i] = 'X';
    }
    *out_offset = copy_end;
    
    if (right_size > 0) {
        decompress_chunk(compressed, offset, output, out_offset);
    }
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk((unsigned char*)input, 0, input_len);
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    
    unsigned char decompressed[MAX_INPUT_SIZE] = {0};
    size_t decompress_offset = 0;
    size_t read_offset = 0;
    
    decompress_chunk(compressed.data, &read_offset, decompressed, &decompress_offset);
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompress_offset && i < MAX_INPUT_SIZE; i++) {
        putchar(decompressed[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}