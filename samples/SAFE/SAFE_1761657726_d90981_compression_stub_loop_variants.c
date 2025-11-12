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

int validate_input(const uint8_t *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
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
            output->length = 0;
            return;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i += count;
    }
    
    output->length = out_idx;
}

void process_with_for_loop(Buffer *buf) {
    for (size_t i = 0; i < buf->length; i++) {
        if (buf->data[i] >= 'a' && buf->data[i] <= 'z') {
            buf->data[i] = buf->data[i] - 'a' + 'A';
        }
    }
}

void process_with_do_while(Buffer *buf) {
    size_t i = 0;
    if (buf->length == 0) return;
    
    do {
        if (buf->data[i] >= 'A' && buf->data[i] <= 'Z') {
            buf->data[i] = buf->data[i] - 'A' + 'a';
        }
        i++;
    } while (i < buf->length);
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer processed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(input.data, temp, len);
    input.length = len;
    
    if (!validate_input(input.data, input.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(processed.data, input.data, input.length);
    processed.length = input.length;
    
    process_with_for_loop(&processed);
    
    printf("Processed (uppercase): ");
    for (size_t i = 0; i < processed.length; i++) {
        putchar(processed.data[i]);
    }
    printf("\n");
    
    process_with_do_while(&processed);
    
    printf("Processed (lowercase): ");
    for (size_t i = 0; i < processed.length; i++) {
        putchar(processed.data[i]);
    }
    printf("\n");
    
    simple_compress(&input, &compressed);
    
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.length);
    for (size_t i = 0; i < compressed.length; i += 2) {
        printf("(%c,%d) ", compressed.data[i], compressed.data[i + 1]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.length / input.length * 100.0f);
    
    return 0;
}