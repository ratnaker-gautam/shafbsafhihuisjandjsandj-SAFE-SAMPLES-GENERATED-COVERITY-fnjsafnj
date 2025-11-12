//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        for (size_t j = i + 1; j < input->size; j++) {
            if (input->data[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        if (output->size + 2 <= MAX_INPUT_SIZE) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
        } else {
            break;
        }
        
        i += count;
    }
}

void decompress_run_length(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        size_t remaining = MAX_INPUT_SIZE - output->size;
        if (count > remaining) {
            count = (uint8_t)remaining;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
        
        i += 2;
    }
}

int get_user_input(Buffer* buffer) {
    char input[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        len = MAX_INPUT_SIZE;
    }
    
    buffer->size = len;
    for (size_t i = 0; i < len; i++) {
        buffer->data[i] = (uint8_t)input[i];
    }
    
    return 1;
}

void display_buffer(const Buffer* buffer, const char* label) {
    printf("%s (%zu bytes): ", label, buffer->size);
    for (size_t i = 0; i < buffer->size; i++) {
        if (isprint(buffer->data[i])) {
            putchar(buffer->data[i]);
        } else {
            printf("[%d]", buffer->data[i]);
        }
    }
    printf("\n");
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&original)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (original.size == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    compress_run_length(&original, &compressed);
    decompress_run_length(&compressed, &decompressed);
    
    printf("\n");
    display_buffer(&original, "Original");
    display_buffer(&compressed, "Compressed");
    display_buffer(&decompressed, "Decompressed");
    
    double ratio = (original.size > 0) ? (double)compressed.size / original.size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Verification: PASS\n");
    } else {
        printf("Verification: FAIL\n");
    }
    
    return 0;
}