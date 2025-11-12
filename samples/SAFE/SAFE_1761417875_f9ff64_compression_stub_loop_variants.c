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

void initialize_buffer(Buffer *buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int read_input(Buffer *input) {
    char temp[MAX_INPUT_SIZE + 2];
    
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

void simple_compress(const Buffer *input, Buffer *output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        if (out_idx >= MAX_OUTPUT_SIZE - 4) {
            break;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input->length; j++) {
            if (input->data[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        if (count > 3 || current == '\\') {
            output->data[out_idx++] = '\\';
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
        } else {
            for (size_t k = 0; k < count; k++) {
                if (out_idx < MAX_OUTPUT_SIZE) {
                    output->data[out_idx++] = current;
                }
            }
        }
        
        i = j;
    }
    
    output->length = out_idx;
}

void display_compression(const Buffer *input, const Buffer *output) {
    printf("Original: ");
    for (size_t i = 0; i < input->length; i++) {
        putchar(input->data[i]);
    }
    printf("\n");
    
    printf("Compressed (%zu bytes): ", output->length);
    for (size_t i = 0; i < output->length; i++) {
        if (isprint(output->data[i]) && output->data[i] != '\\') {
            putchar(output->data[i]);
        } else {
            printf("\\x%02X", output->data[i]);
        }
    }
    printf("\n");
    
    if (output->length < input->length) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)output->length / input->length) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
}

int main(void) {
    Buffer input, output;
    
    initialize_buffer(&input);
    initialize_buffer(&output);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Invalid input or input too long\n");
        return 1;
    }
    
    if (input.length == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    simple_compress(&input, &output);
    display_compression(&input, &output);
    
    return 0;
}