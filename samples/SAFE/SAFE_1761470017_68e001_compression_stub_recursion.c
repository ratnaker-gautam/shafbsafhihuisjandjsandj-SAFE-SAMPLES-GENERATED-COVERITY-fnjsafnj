//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = data[start];
        result.size = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = compress_chunk(data, start, mid);
    Buffer right = compress_chunk(data, mid, end);
    
    size_t total_size = left.size + right.size;
    if (total_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = total_size;
    
    return result;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void print_buffer(const Buffer* buf) {
    if (buf == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buf->size > 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "Invalid byte value\n");
            return 1;
        }
        if (value > 0xFF) {
            fprintf(stderr, "Byte value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_buffer(&(Buffer){input_data, input_size});
    
    Buffer compressed = compress_chunk(input_data, 0, input_size);
    
    printf("\nProcessed data (%zu bytes):\n", compressed.size);
    print_buffer(&compressed);
    
    if (compressed.size > 0) {
        double ratio = (double)compressed.size / (double)input_size * 100.0;
        printf("Size ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}