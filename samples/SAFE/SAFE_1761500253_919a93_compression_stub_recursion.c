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

void compress_chunk_recursive(const uint8_t* input, size_t start, size_t end, Buffer* output) {
    if (start >= end) {
        return;
    }
    
    if (end - start == 1) {
        if (output->size < MAX_INPUT_SIZE - 2) {
            output->data[output->size++] = 0x01;
            output->data[output->size++] = input[start];
        }
        return;
    }
    
    size_t run_length = 1;
    size_t current = start + 1;
    
    while (current < end && input[current] == input[start] && run_length < 255) {
        run_length++;
        current++;
    }
    
    if (run_length > 1) {
        if (output->size < MAX_INPUT_SIZE - 2) {
            output->data[output->size++] = (uint8_t)run_length;
            output->data[output->size++] = input[start];
        }
        compress_chunk_recursive(input, start + run_length, end, output);
    } else {
        size_t literal_start = start;
        size_t literal_end = start + 1;
        
        while (literal_end < end && input[literal_end] != input[literal_end - 1] && 
               (literal_end - literal_start) < 255) {
            literal_end++;
        }
        
        size_t literal_length = literal_end - literal_start;
        if (output->size < MAX_INPUT_SIZE - literal_length - 1) {
            output->data[output->size++] = (uint8_t)(0x80 | literal_length);
            for (size_t i = 0; i < literal_length; i++) {
                output->data[output->size++] = input[literal_start + i];
            }
        }
        compress_chunk_recursive(input, literal_start + literal_length, end, output);
    }
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    output->size = 0;
    compress_chunk_recursive(input, 0, input_size, output);
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if (i + 1 < buf->size) {
            printf(" ");
        }
    }
    if (buf->size > 0 && buf->size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    Buffer input_data = {0};
    Buffer compressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
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
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= sizeof(input_data.data)) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_data.data, input_str, len);
    input_data.size = len;
    
    printf("Original data (%zu bytes):\n", input_data.size);
    for (size_t i = 0; i < input_data.size; i++) {
        printf("%c", input_data.data[i]);
    }
    printf("\n");
    
    if (compress_data(input_data.data, input_data.size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    double ratio = (input_data.size > 0) ? (double)compressed.size / input_data.size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}