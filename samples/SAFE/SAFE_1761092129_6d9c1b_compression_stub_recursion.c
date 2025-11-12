//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

void compress_chunk(Buffer* input, Buffer* output, size_t pos) {
    if (pos >= input->size) {
        return;
    }
    
    uint8_t current = input->data[pos];
    size_t count = 1;
    
    size_t next_pos = pos + 1;
    while (next_pos < input->size && input->data[next_pos] == current && count < 255) {
        count++;
        next_pos++;
    }
    
    if (output->size + 2 <= MAX_INPUT_SIZE) {
        output->data[output->size++] = current;
        output->data[output->size++] = (uint8_t)count;
    }
    
    compress_chunk(input, output, next_pos);
}

int compress_data(Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    output->size = 0;
    compress_chunk(input, output, 0);
    
    return 0;
}

void print_buffer(Buffer* buf) {
    if (buf == NULL || buf->size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input = {0};
    Buffer output = {0};
    
    printf("Enter data to compress (max %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \n");
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) == 1 && value <= 0xFF) {
            input.data[input.size++] = (uint8_t)value;
        } else {
            printf("Invalid hex value: %s\n", token);
            return 1;
        }
        token = strtok(NULL, " \n");
    }
    
    if (input.size == 0) {
        printf("No valid input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input.size);
    print_buffer(&input);
    
    if (compress_data(&input, &output) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", output.size);
    print_buffer(&output);
    
    if (output.size > 0) {
        double ratio = (double)output.size / (double)input.size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}