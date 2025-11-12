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
    
    if (output.size + 2 > MAX_INPUT_SIZE * 2) {
        return output;
    }
    
    size_t count = 1;
    while (pos + count < input_size && count < UCHAR_MAX && input[pos] == input[pos + count]) {
        count++;
    }
    
    if (output.size + 2 <= MAX_INPUT_SIZE * 2) {
        output.data[output.size++] = (unsigned char)count;
        output.data[output.size++] = input[pos];
    }
    
    return compress_rle(input, input_size, pos + count, output);
}

int validate_input(const unsigned char* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char output_data[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Buffer output = {output_data, 0};
    output = compress_rle(input, input_len, 0, output);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compression ratio: %.2f%%\n", output.size * 100.0 / input_len);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output.size; i++) {
        printf("%02X ", output.data[i]);
    }
    printf("\n");
    
    return 0;
}