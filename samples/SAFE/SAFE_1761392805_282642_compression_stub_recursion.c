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
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < input_size && input[pos + count] == current && count < UCHAR_MAX) {
        count++;
    }
    
    if (output.data == NULL) {
        output.data = malloc(MAX_INPUT_SIZE * 2);
        if (output.data == NULL) {
            output.size = 0;
            return output;
        }
    }
    
    if (output.size + 2 <= MAX_INPUT_SIZE * 2) {
        output.data[output.size++] = (unsigned char)count;
        output.data[output.size++] = current;
    }
    
    return compress_rle(input, input_size, pos + count, output);
}

int validate_input(const char* input, size_t length) {
    if (input == NULL || length == 0 || length > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void print_compressed(const Buffer* compressed) {
    if (compressed == NULL || compressed->data == NULL || compressed->size == 0) {
        printf("No compressed data to display.\n");
        return;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed->size);
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 < compressed->size) {
            printf("[%d x 0x%02X] ", compressed->data[i], compressed->data[i + 1]);
        }
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    size_t input_length;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n') {
        input[--input_length] = '\0';
    }
    
    if (!validate_input(input, input_length)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_length);
    
    Buffer compressed = {NULL, 0};
    compressed = compress_rle((unsigned char*)input, input_length, 0, compressed);
    
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    print_compressed(&compressed);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_length * 100.0f);
    
    free(compressed.data);
    return 0;
}