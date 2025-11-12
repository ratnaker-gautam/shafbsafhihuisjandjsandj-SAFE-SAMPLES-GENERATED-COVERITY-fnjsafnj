//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

int compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return -1;
    
    unsigned char *out_ptr = output;
    const unsigned char *in_ptr = input;
    const unsigned char *end_ptr = input + input_size;
    
    while (in_ptr < end_ptr) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next = in_ptr + 1;
        while (next < end_ptr && *next == current && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if ((out_ptr - output) + 2 > MAX_OUTPUT_SIZE) return -1;
        
        *out_ptr++ = current;
        *out_ptr++ = (unsigned char)count;
        
        in_ptr += count;
    }
    
    *output_size = out_ptr - output;
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) return -1;
    
    unsigned char *out_ptr = output;
    const unsigned char *in_ptr = input;
    const unsigned char *end_ptr = input + input_size;
    
    while (in_ptr < end_ptr - 1) {
        unsigned char value = *in_ptr++;
        unsigned char count = *in_ptr++;
        
        if ((out_ptr - output) + count > MAX_INPUT_SIZE) return -1;
        
        for (unsigned char i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    *output_size = out_ptr - output;
    return 0;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char compressed[MAX_OUTPUT_SIZE];
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_size);
    
    size_t compressed_size = 0;
    if (compress_rle(input, input_size, compressed, &compressed_size) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_size) * 100);
    
    size_t decompressed_size = 0;
    if (decompress_rle(compressed, compressed_size, decompressed, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed[decompressed_size] = '\0';
    
    if (decompressed_size != input_size || memcmp(input, decompressed, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", decompressed);
    printf("Decompression successful\n");
    
    return 0;
}