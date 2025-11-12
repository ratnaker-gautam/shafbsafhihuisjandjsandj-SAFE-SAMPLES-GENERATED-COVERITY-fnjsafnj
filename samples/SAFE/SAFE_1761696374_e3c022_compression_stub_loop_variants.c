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

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    output->length = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input->length; j++) {
            if (input->data[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        if (output->length + 3 > MAX_OUTPUT_SIZE) break;
        
        output->data[output->length++] = current;
        output->data[output->length++] = (uint8_t)(count & 0xFF);
        
        i = j;
    }
}

void process_input() {
    Buffer input = {0};
    Buffer output = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE + 1];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    for (size_t k = 0; k < len; k++) {
        input.data[k] = (uint8_t)temp[k];
    }
    input.length = len;
    
    if (!validate_input(input.data, input.length)) {
        printf("Invalid input\n");
        return;
    }
    
    simple_compress(&input, &output);
    
    printf("Original: ");
    for (size_t m = 0; m < input.length; m++) {
        putchar(input.data[m]);
    }
    printf(" (%zu bytes)\n", input.length);
    
    printf("Compressed: ");
    size_t n = 0;
    do {
        if (n < output.length) {
            printf("%02X", output.data[n]);
        }
        n++;
    } while (n < output.length);
    printf(" (%zu bytes)\n", output.length);
    
    if (output.length < input.length) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)output.length / input.length) * 100);
    } else {
        printf("No compression achieved\n");
    }
}

int main() {
    process_input();
    return 0;
}