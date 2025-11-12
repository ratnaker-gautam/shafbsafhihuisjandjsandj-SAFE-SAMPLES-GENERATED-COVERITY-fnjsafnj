//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    output->length = out_idx;
}

void process_input() {
    Buffer input, output;
    initialize_buffer(&input);
    initialize_buffer(&output);
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t pos = 0;
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE - 1) {
        input.data[pos++] = (uint8_t)c;
    }
    input.length = pos;
    
    if (!validate_input(input.data, input.length)) {
        printf("Invalid input\n");
        return;
    }
    
    simple_compress(&input, &output);
    
    printf("Original size: %zu bytes\n", input.length);
    printf("Compressed size: %zu bytes\n", output.length);
    printf("Compression ratio: %.2f%%\n", 
           (input.length > 0) ? (100.0 * output.length / input.length) : 0.0);
    
    printf("Compressed data: ");
    for (size_t k = 0; k < output.length; k += 2) {
        if (k > 0) printf(" ");
        printf("%02X%02X", output.data[k], output.data[k + 1]);
    }
    printf("\n");
}

int main() {
    process_input();
    return 0;
}