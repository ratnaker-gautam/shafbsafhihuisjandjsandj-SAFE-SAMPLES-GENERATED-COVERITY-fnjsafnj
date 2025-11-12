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

int decompress_rle(const unsigned char* input, size_t input_size, size_t pos, unsigned char* output, size_t* output_pos) {
    if (pos >= input_size) {
        return 0;
    }
    
    if (pos + 1 >= input_size) {
        return -1;
    }
    
    unsigned char count = input[pos];
    unsigned char value = input[pos + 1];
    
    if (*output_pos + count > MAX_INPUT_SIZE) {
        return -1;
    }
    
    for (unsigned char i = 0; i < count; i++) {
        output[(*output_pos)++] = value;
    }
    
    return decompress_rle(input, input_size, pos + 2, output, output_pos);
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
    
    Buffer comp_buffer;
    comp_buffer.data = compressed;
    comp_buffer.size = 0;
    
    comp_buffer = compress_rle(input, input_size, 0, comp_buffer);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", comp_buffer.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)comp_buffer.size / input_size) * 100);
    
    size_t decomp_pos = 0;
    int result = decompress_rle(compressed, comp_buffer.size, 0, decompressed, &decomp_pos);
    
    if (result != 0 || decomp_pos != input_size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input, decompressed, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful\n");
    printf("Original: %s\n", input);
    printf("Decompressed: %s\n", decompressed);
    
    return 0;
}