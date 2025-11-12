//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (output->size + 2 > MAX_INPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->size && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
        output->size += 2;
        
        i = j;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0 || input->size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (output->size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        size_t k = 0;
        do {
            output->data[output->size + k] = value;
            k++;
        } while (k < count);
        
        output->size += count;
    }
    
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char input_str[MAX_INPUT_SIZE];
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    original.size = len;
    for (size_t i = 0; i < len; i++) {
        original.data[i] = (uint8_t)input_str[i];
    }
    
    printf("Original: ");
    print_buffer_hex(&original);
    
    if (compress_rle(&original, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed: ");
    print_buffer_hex(&compressed);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    print_buffer_hex(&decompressed);
    
    if (original.size != decompressed.size) {
        fprintf(stderr, "Size mismatch\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < original.size; i++) {
        if (original.data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Compression/Decompression successful\n");
    } else {
        fprintf(stderr, "Data mismatch\n");
        return 1;
    }
    
    return 0;
}