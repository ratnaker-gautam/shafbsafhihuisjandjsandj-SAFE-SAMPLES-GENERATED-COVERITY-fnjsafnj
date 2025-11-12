//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
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
        
        i += count;
    }
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
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
        
        size_t k = 0;
        while (k < count) {
            output->data[output->size] = value;
            output->size++;
            k++;
        }
    }
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len >= max_size) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)temp[i];
    }
    buffer[len] = '\0';
    
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
        printf("%c", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    int input_length = get_user_input(input_data, MAX_INPUT_SIZE);
    
    if (input_length <= 0) {
        printf("Invalid input or input too long\n");
        return 1;
    }
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    compress_rle(input_data, (size_t)input_length, &compressed);
    
    printf("Original text: ");
    print_buffer_text((Buffer*)&input_data);
    
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_length * 100.0f);
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    printf("Decompressed text: ");
    print_buffer_text(&decompressed);
    
    if (memcmp(input_data, decompressed.data, (size_t)input_length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    return 0;
}