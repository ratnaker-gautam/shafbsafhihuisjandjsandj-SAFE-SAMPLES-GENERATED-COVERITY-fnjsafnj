//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

Buffer compress_rle(const unsigned char *input, size_t input_size, size_t pos, Buffer output) {
    if (pos >= input_size) {
        return output;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < input_size && input[pos + count] == current && count < UCHAR_MAX) {
        count++;
    }
    
    if (output.size + 2 > MAX_INPUT_SIZE * 2) {
        return output;
    }
    
    output.data[output.size++] = current;
    output.data[output.size++] = (unsigned char)count;
    
    return compress_rle(input, input_size, pos + count, output);
}

int decompress_rle(const unsigned char *input, size_t input_size, size_t pos, unsigned char *output, size_t *output_size) {
    if (pos >= input_size) {
        return 1;
    }
    
    if (pos + 1 >= input_size) {
        return 0;
    }
    
    unsigned char value = input[pos];
    unsigned char count = input[pos + 1];
    
    if (*output_size + count > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        output[(*output_size)++] = value;
    }
    
    return decompress_rle(input, input_size, pos + 2, output, output_size);
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char compressed[MAX_INPUT_SIZE * 2];
    unsigned char decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer comp_buffer;
    comp_buffer.data = compressed;
    comp_buffer.size = 0;
    
    comp_buffer = compress_rle(input, input_len, 0, comp_buffer);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", comp_buffer.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)comp_buffer.size / input_len) * 100);
    
    size_t decomp_size = 0;
    if (!decompress_rle(compressed, comp_buffer.size, 0, decompressed, &decomp_size)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed[decomp_size] = '\0';
    
    if (decomp_size != input_len || memcmp(input, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed successfully: %s\n", decompressed);
    
    return 0;
}