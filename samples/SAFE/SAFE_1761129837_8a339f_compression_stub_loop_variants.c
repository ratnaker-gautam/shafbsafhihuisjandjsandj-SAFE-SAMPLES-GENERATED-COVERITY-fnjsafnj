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

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (output->size + 2 > MAX_INPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0 || 
        input->size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    for (i = 0; i < input->size; i += 2) {
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (output->size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        size_t j = 0;
        do {
            output->data[output->size + j] = value;
            j++;
        } while (j < count);
        
        output->size += count;
    }
    
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input, compressed, decompressed;
    char user_input[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    input.size = len;
    for (size_t i = 0; i < len; i++) {
        input.data[i] = (uint8_t)user_input[i];
    }
    
    printf("Original: ");
    print_buffer_hex(&input);
    
    if (compress_rle(&input, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed: ");
    print_buffer_hex(&compressed);
    printf("Compression ratio: %.2f\n", 
           (float)input.size / (compressed.size > 0 ? compressed.size : 1));
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        fprintf(stderr, "Size mismatch\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input.size; i++) {
        if (input.data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}