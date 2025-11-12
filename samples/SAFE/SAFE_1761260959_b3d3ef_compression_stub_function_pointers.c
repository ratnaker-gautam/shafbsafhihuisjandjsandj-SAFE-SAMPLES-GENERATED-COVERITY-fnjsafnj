//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(Buffer);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input.size && 
               input.data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer decompress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[in_pos++];
        uint8_t value = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

Buffer process_data(Buffer input, CompressionFunc func) {
    if (func == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    return func(input);
}

void print_buffer(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf.size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            printf("Invalid byte value\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    Buffer input = {input_data, input_size};
    
    CompressionFunc compressors[] = {compress_rle, decompress_rle};
    const char *names[] = {"compress", "decompress"};
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_buffer(input);
    
    for (int i = 0; i < 2; i++) {
        printf("\n%s result:\n", names[i]);
        Buffer result = process_data(input, compressors[i]);
        
        if (result.data != NULL && result.size > 0) {
            printf("Size: %zu bytes\n", result.size);
            print_buffer(result);
            free(result.data);
        } else {
            printf("Processing failed\n");
        }
    }
    
    return 0;
}