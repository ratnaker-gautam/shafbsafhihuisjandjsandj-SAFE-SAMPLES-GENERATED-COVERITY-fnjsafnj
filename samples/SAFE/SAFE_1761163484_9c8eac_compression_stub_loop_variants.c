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
        input->data[i] = (uint8_t)temp[i];
    }
    
    input->length = len;
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
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
        
        i += count;
    }
    
    output->length = out_idx;
}

void display_compressed(const Buffer *compressed) {
    printf("Compressed data (%zu bytes): ", compressed->length);
    
    for (size_t idx = 0; idx < compressed->length; idx++) {
        if (idx % 2 == 0) {
            printf("[%u", (unsigned int)compressed->data[idx]);
        } else {
            printf(":'%c'] ", compressed->data[idx]);
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input, compressed;
    
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Invalid input or input too long\n");
        return 1;
    }
    
    if (input.length == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    simple_compress(&input, &compressed);
    
    printf("Original: %s\n", input.data);
    printf("Original size: %zu bytes\n", input.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    if (compressed.length > 0) {
        display_compressed(&compressed);
    }
    
    double ratio = (input.length > 0) ? 
        (double)compressed.length / input.length * 100.0 : 0.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}