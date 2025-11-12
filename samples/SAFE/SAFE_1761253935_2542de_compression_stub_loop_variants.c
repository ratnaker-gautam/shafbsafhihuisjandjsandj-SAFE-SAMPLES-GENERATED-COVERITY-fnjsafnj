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

int read_input(Buffer* input) {
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

void compress_run_length(Buffer* input, Buffer* output) {
    if (input->length == 0) {
        output->length = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        if (out_idx + 3 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count >= 3 || current == '1' || current == '2' || current == '3') {
            if (out_idx + 3 < MAX_OUTPUT_SIZE) {
                output->data[out_idx++] = '1';
                output->data[out_idx++] = '2';
                output->data[out_idx++] = '3';
                output->data[out_idx++] = current;
                output->data[out_idx++] = (uint8_t)count;
            }
        } else {
            for (size_t k = 0; k < count && out_idx < MAX_OUTPUT_SIZE; k++) {
                output->data[out_idx++] = current;
            }
        }
        
        i = j;
    }
    
    output->length = out_idx;
}

void display_output(Buffer* output) {
    for (size_t i = 0; i < output->length; i++) {
        if (isprint(output->data[i]) || output->data[i] == ' ') {
            putchar(output->data[i]);
        } else {
            printf("[%d]", output->data[i]);
        }
    }
    putchar('\n');
}

int main(void) {
    Buffer input, output;
    
    initialize_buffer(&input);
    initialize_buffer(&output);
    
    printf("Enter text to compress: ");
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    compress_run_length(&input, &output);
    
    printf("Compressed: ");
    display_output(&output);
    
    size_t original_size = input.length;
    size_t compressed_size = output.length;
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (original_size > 0) {
        double ratio = (double)compressed_size / original_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}