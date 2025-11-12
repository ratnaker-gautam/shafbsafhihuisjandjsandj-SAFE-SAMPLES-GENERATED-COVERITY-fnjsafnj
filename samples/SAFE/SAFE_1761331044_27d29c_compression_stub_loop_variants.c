//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    size_t i = 0;
    output->length = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (output->length + 2 > MAX_OUTPUT_SIZE) break;
        
        output->data[output->length++] = (uint8_t)count;
        output->data[output->length++] = current;
        
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    output->length = 0;
    
    for (size_t i = 0; i < input->length; i += 2) {
        if (i + 1 >= input->length) break;
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (output->length + count > MAX_OUTPUT_SIZE) break;
        
        size_t k = 0;
        do {
            output->data[output->length++] = value;
            k++;
        } while (k < count);
    }
}

int buffers_equal(const Buffer* a, const Buffer* b) {
    if (a == NULL || b == NULL) return 0;
    if (a->length != b->length) return 0;
    
    for (size_t i = 0; i < a->length; i++) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->length > 0) printf("\n");
}

int main(void) {
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input_line[MAX_INPUT_SIZE + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    original.length = input_len;
    for (size_t i = 0; i < input_len; i++) {
        original.data[i] = (uint8_t)input_line[i];
    }
    
    if (!validate_input(original.data, original.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", original.length);
    print_buffer_hex(&original);
    
    simple_compress(&original, &compressed);
    printf("\nCompressed data (%zu bytes):\n", compressed.length);
    print_buffer_hex(&compressed);
    
    simple_decompress(&compressed, &decompressed);
    printf("\nDecompressed data (%zu bytes):\n", decompressed.length);
    print_buffer_hex(&decompressed);
    
    if (buffers_equal(&original, &decompressed)) {
        printf("\nCompression successful: original == decompressed\n");
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.length / original.length) * 100.0);
    } else {
        printf("\nCompression failed: original != decompressed\n");
        return 1;
    }
    
    return 0;
}