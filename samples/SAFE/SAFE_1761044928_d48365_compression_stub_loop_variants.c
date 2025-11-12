//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

int read_input(Buffer* input) {
    char temp[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)temp[i]) && temp[i] != ' ') {
            return 0;
        }
    }
    
    memcpy(input->data, temp, len);
    input->length = len;
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->length == 0) {
        output->length = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i = j;
    }
    
    output->length = out_idx;
}

void display_compression(const Buffer* input, const Buffer* output) {
    printf("\nOriginal text (%zu bytes):\n", input->length);
    for (size_t i = 0; i < input->length; i++) {
        putchar(input->data[i]);
    }
    
    printf("\n\nCompressed data (%zu bytes):\n", output->length);
    for (size_t i = 0; i < output->length; i += 2) {
        if (i + 1 < output->length) {
            printf("[%d '%c'] ", output->data[i], output->data[i + 1]);
        }
    }
    
    if (output->length > 0) {
        double ratio = (input->length > 0) ? (100.0 * (input->length - output->length) / input->length) : 0.0;
        printf("\n\nCompression ratio: %.1f%%", ratio);
    }
    printf("\n");
}

int main(void) {
    Buffer input, output;
    
    initialize_buffer(&input);
    initialize_buffer(&output);
    
    printf("Simple Run-Length Compression Demo\n");
    printf("==================================\n");
    
    if (!read_input(&input)) {
        printf("Error: Invalid input or input too large\n");
        return 1;
    }
    
    if (input.length == 0) {
        printf("Error: No input provided\n");
        return 1;
    }
    
    simple_compress(&input, &output);
    display_compression(&input, &output);
    
    return 0;
}