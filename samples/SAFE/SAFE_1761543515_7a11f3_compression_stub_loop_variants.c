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

void decompress_run_length(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size == 0) {
        *output_size = 0;
        return;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size + j] = value;
        }
        *output_size += count;
        
        i += 2;
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

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) {
        printf("(empty)\n");
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void print_buffer_text(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) {
        printf("(empty)\n");
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (isprint(data[i])) {
            printf("%c", data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_size;
    
    int input_length = get_user_input(input, sizeof(input));
    if (input_length <= 0) {
        printf("Invalid input or input too long\n");
        return 1;
    }
    
    printf("\nOriginal text (%d bytes):\n", input_length);
    print_buffer_text(input, (size_t)input_length);
    
    compress_run_length(input, (size_t)input_length, &compressed);
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    decompress_run_length(&compressed, decompressed, &decompressed_size);
    printf("\nDecompressed text (%zu bytes):\n", decompressed_size);
    print_buffer_text(decompressed, decompressed_size);
    
    if (decompressed_size == (size_t)input_length && 
        memcmp(input, decompressed, decompressed_size) == 0) {
        printf("\nCompression/Decompression successful!\n");
        
        if (compressed.size < (size_t)input_length) {
            float ratio = ((float)compressed.size / (float)input_length) * 100.0f;
            printf("Compression ratio: %.1f%%\n", ratio);
        } else {
            printf("No compression achieved (data expanded)\n");
        }
    } else {
        printf("\nCompression/Decompression failed!\n");
        return 1;
    }
    
    return 0;
}