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

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[output->size++] = current;
        output->data[output->size++] = (uint8_t)count;
        i += count;
    }
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output[*output_size + k] = value;
            k++;
        } while (k < count);
        
        *output_size += count;
    }
}

int get_user_input(uint8_t* buffer, size_t* size) {
    char input[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
        buffer[i] = (uint8_t)input[i];
    }
    
    *size = len;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

void print_buffer_text(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    if (!get_user_input(input_data, &input_size)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input_size == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed;
    compress_rle(input_data, input_size, &compressed);
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f\n", (float)compressed.size / input_size);
    
    printf("Compressed data (hex): ");
    print_buffer_hex(&compressed);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = 0;
    decompress_rle(&compressed, decompressed, &decompressed_size);
    
    printf("Decompressed text: ");
    print_buffer_text(decompressed, decompressed_size);
    
    if (decompressed_size == input_size && 
        memcmp(input_data, decompressed, input_size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    return 0;
}