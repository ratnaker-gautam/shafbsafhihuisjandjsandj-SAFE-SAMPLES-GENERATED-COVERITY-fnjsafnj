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
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    char temp_buffer[MAX_INPUT_SIZE];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp_buffer);
    if (len > 0 && temp_buffer[len - 1] == '\n') {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= max_size) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)temp_buffer[i];
    }
    
    return (int)len;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

void print_buffer_text(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        if (isprint(buffer->data[i])) {
            printf("%c", buffer->data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    int input_size = get_user_input(input_data, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Invalid input or input too large\n");
        return 1;
    }
    
    printf("\nOriginal text (%d bytes): ", input_size);
    for (int i = 0; i < input_size; i++) {
        printf("%c", input_data[i]);
    }
    printf("\n");
    
    compress_run_length(input_data, (size_t)input_size, &compressed);
    printf("Compressed data (%zu bytes, hex): ", compressed.size);
    print_buffer_hex(&compressed);
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    printf("Decompressed text (%zu bytes): ", decompressed.size);
    print_buffer_text(&decompressed);
    
    if (decompressed.size == (size_t)input_size && 
        memcmp(input_data, decompressed.data, decompressed.size) == 0) {
        printf("Compression/Decompression successful!\n");
        double ratio = (double)compressed.size / (double)input_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("Compression/Decompression failed!\n");
        return 1;
    }
    
    return 0;
}