//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        size_t j = in_idx + 1;
        while (j < input->size && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_INPUT_SIZE) {
            break;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        in_idx += count;
    }
    
    output->size = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    output->size = out_idx;
}

int main(void) {
    char user_input[MAX_INPUT_SIZE + 1];
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    original.size = input_len;
    for (size_t i = 0; i < input_len; i++) {
        original.data[i] = (uint8_t)user_input[i];
    }
    
    compressed.size = 0;
    simple_compress(&original, &compressed);
    
    decompressed.size = 0;
    simple_decompress(&compressed, &decompressed);
    
    printf("Original size: %zu bytes\n", original.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed.size * 100.0) / (original.size > 0 ? original.size : 1));
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    int verify_ok = 1;
    if (original.size != decompressed.size) {
        verify_ok = 0;
    } else {
        for (size_t i = 0; i < original.size; i++) {
            if (original.data[i] != decompressed.data[i]) {
                verify_ok = 0;
                break;
            }
        }
    }
    
    if (verify_ok) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    return 0;
}