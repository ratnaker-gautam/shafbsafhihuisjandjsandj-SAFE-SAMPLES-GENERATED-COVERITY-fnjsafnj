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
        
        output->data[output->size++] = current;
        output->data[output->size++] = (uint8_t)count;
        
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
            output->data[output->size++] = value;
            k++;
        } while (k < count);
    }
    
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
        }
    }
    if (buf->size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input, compressed, decompressed;
    char input_str[MAX_INPUT_SIZE];
    
    printf("Enter text to compress: ");
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
    
    input.size = len;
    for (size_t i = 0; i < len; i++) {
        input.data[i] = (uint8_t)input_str[i];
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(&input);
    
    if (compress_rle(&input, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    float ratio = (compressed.size == 0) ? 0.0f : (float)input.size / compressed.size;
    printf("Compression ratio: %.2f:1\n", ratio);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input.size; i++) {
        if (input.data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: data matches original\n");
    } else {
        fprintf(stderr, "Decompression failed: data mismatch\n");
        return 1;
    }
    
    return 0;
}