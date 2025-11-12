//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)
#define RUN_LENGTH_MAX 255
#define BYTE_MASK 0xFF

typedef struct {
    unsigned char data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const unsigned char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_rle_compress(const unsigned char *input, size_t len, Buffer *output) {
    output->size = 0;
    if (len == 0) return;
    
    size_t i = 0;
    while (i < len) {
        unsigned char current = input[i];
        unsigned char count = 1;
        
        while (count < RUN_LENGTH_MAX && i + count < len && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) break;
        
        output->data[output->size++] = count;
        output->data[output->size++] = current;
        i += count;
    }
}

static void simple_rle_decompress(const Buffer *input, unsigned char *output, size_t *out_len) {
    *out_len = 0;
    if (input->size == 0) return;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) break;
        
        unsigned char count = input->data[i];
        unsigned char value = input->data[i + 1];
        
        if (*out_len + count > MAX_INPUT_SIZE) break;
        
        for (unsigned char j = 0; j < count; j++) {
            output[(*out_len)++] = value;
        }
    }
}

static int buffers_equal(const unsigned char *a, const unsigned char *b, size_t len) {
    if (a == NULL || b == NULL) return 0;
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_len = 0;
    size_t decompressed_len = 0;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_len >= MAX_INPUT_SIZE - 1) break;
        input[input_len++] = (unsigned char)c;
    }
    input[input_len] = '\0';
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    simple_rle_compress(input, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_len - compressed.size) * 100.0f / (float)input_len);
    }
    
    simple_rle_decompress(&compressed, decompressed, &decompressed_len);
    
    if (decompressed_len != input_len || 
        !buffers_equal(input, decompressed, input_len)) {
        fprintf(stderr, "Decompression verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully\n");
    
    if (compressed.size > 0) {
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02X ", compressed.data[i]);
        }
        printf("\n");
    }
    
    return EXIT_SUCCESS;
}