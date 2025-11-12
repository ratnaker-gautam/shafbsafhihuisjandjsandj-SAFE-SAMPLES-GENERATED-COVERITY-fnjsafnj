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
            output->length = 0;
            return;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i += count;
    }
    
    output->length = out_idx;
}

void display_compressed(const Buffer *compressed) {
    for (size_t i = 0; i < compressed->length; i += 2) {
        if (i + 1 < compressed->length) {
            printf("%c%u", compressed->data[i], (unsigned int)compressed->data[i + 1]);
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input, compressed;
    
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (input.length == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    
    if (compressed.length == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed: ");
    display_compressed(&compressed);
    
    size_t original_size = input.length;
    size_t compressed_size = compressed.length;
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size < original_size) {
        double ratio = (double)compressed_size / original_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("No compression achieved\n");
    }
    
    return 0;
}