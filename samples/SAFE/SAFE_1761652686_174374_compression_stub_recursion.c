//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
} Buffer;

Buffer compress_rle(const unsigned char* input, size_t input_size, size_t pos, Buffer output) {
    if (pos >= input_size) {
        return output;
    }
    
    size_t count = 1;
    while (pos + count < input_size && input[pos] == input[pos + count] && count < UCHAR_MAX) {
        count++;
    }
    
    if (output.size + 2 > MAX_INPUT_SIZE * 2) {
        return output;
    }
    
    output.data[output.size++] = (unsigned char)count;
    output.data[output.size++] = input[pos];
    
    return compress_rle(input, input_size, pos + count, output);
}

void decompress_rle(const unsigned char* compressed, size_t compressed_size, size_t pos, unsigned char* output, size_t* output_pos) {
    if (pos >= compressed_size || *output_pos >= MAX_INPUT_SIZE) {
        return;
    }
    
    unsigned char count = compressed[pos];
    unsigned char value = compressed[pos + 1];
    
    for (unsigned char i = 0; i < count && *output_pos < MAX_INPUT_SIZE; i++) {
        output[(*output_pos)++] = value;
    }
    
    decompress_rle(compressed, compressed_size, pos + 2, output, output_pos);
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
    
    size_t input_size = strlen((char*)input);
    if (input_size > 0 && input[input_size - 1] == '\n') {
        input[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer output_buffer;
    output_buffer.data = compressed;
    output_buffer.size = 0;
    
    output_buffer = compress_rle(input, input_size, 0, output_buffer);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", output_buffer.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)output_buffer.size / input_size) * 100);
    
    size_t decompressed_pos = 0;
    decompress_rle(compressed, output_buffer.size, 0, decompressed, &decompressed_pos);
    
    if (decompressed_pos != input_size || memcmp(input, decompressed, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: %s\n", input);
    printf("Decompressed: %s\n", decompressed);
    
    return 0;
}