//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i = j;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    for (size_t idx = 0; idx < input_size; idx += 2) {
        uint8_t count = input[idx];
        uint8_t value = input[idx + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t c = 0; c < count; c++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
}

int validate_input(const char* input, size_t* input_size) {
    if (input == NULL || input_size == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) {
            return 0;
        }
    }
    
    *input_size = len;
    return 1;
}

void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        if (isprint(buffer->data[i])) {
            putchar(buffer->data[i]);
        } else {
            printf("[0x%02X]", buffer->data[i]);
        }
    }
    printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    size_t input_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input_str, &input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    compress_run_length((uint8_t*)input_str, input_size, &compressed);
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data: ");
    print_buffer(&compressed);
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == input_size && 
        memcmp(decompressed.data, input_str, input_size) == 0) {
        printf("Decompression successful: ");
        for (size_t i = 0; i < decompressed.size; i++) {
            putchar(decompressed.data[i]);
        }
        printf("\n");
    } else {
        printf("Decompression failed\n");
        return 1;
    }
    
    return 0;
}