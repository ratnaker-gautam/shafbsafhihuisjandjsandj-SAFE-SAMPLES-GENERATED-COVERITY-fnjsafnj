//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t pos;
} output_buffer;

void compress_rle(const uint8_t* input, size_t len, output_buffer* out, size_t idx) {
    if (idx >= len) return;
    
    uint8_t current = input[idx];
    size_t count = 1;
    
    while (idx + count < len && input[idx + count] == current && count < 255) {
        count++;
    }
    
    if (out->pos + 2 <= MAX_OUTPUT_SIZE) {
        out->data[out->pos++] = current;
        out->data[out->pos++] = (uint8_t)count;
    }
    
    compress_rle(input, len, out, idx + count);
}

int validate_input(const uint8_t* data, size_t len) {
    if (data == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    output_buffer output = {0};
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n' && bytes_read < MAX_INPUT_SIZE) {
        input[bytes_read++] = (uint8_t)ch;
    }
    
    if (!validate_input(input, bytes_read)) {
        printf("Invalid input\n");
        return 1;
    }
    
    compress_rle(input, bytes_read, &output, 0);
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", output.pos);
    printf("Compression ratio: %.2f%%\n", 
           (bytes_read > 0) ? (100.0 * output.pos / bytes_read) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output.pos; i++) {
        printf("%02X ", output.data[i]);
    }
    printf("\n");
    
    return 0;
}