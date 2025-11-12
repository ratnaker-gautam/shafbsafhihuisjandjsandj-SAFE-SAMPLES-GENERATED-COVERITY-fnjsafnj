//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
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

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

void compress_run_length_recursive(const uint8_t* input, size_t len, size_t pos, Buffer* output, size_t* out_pos) {
    if (pos >= len || *out_pos >= MAX_INPUT_SIZE - 2) {
        return;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (*out_pos + 2 < MAX_INPUT_SIZE) {
        output->data[(*out_pos)++] = current;
        output->data[(*out_pos)++] = (uint8_t)count;
    }
    
    compress_run_length_recursive(input, len, pos + count, output, out_pos);
}

int compress_data(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len)) {
        return 0;
    }
    
    output->size = 0;
    size_t out_pos = 0;
    
    compress_run_length_recursive((const uint8_t*)input, input_len, 0, output, &out_pos);
    output->size = out_pos;
    
    return 1;
}

void decompress_data(const Buffer* compressed, char* output, size_t* output_len) {
    if (compressed == NULL || output == NULL || output_len == NULL) {
        *output_len = 0;
        return;
    }
    
    size_t out_pos = 0;
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 >= compressed->size) break;
        
        uint8_t character = compressed->data[i];
        uint8_t count = compressed->data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (out_pos < MAX_INPUT_SIZE - 1) {
                output[out_pos++] = character;
            } else {
                break;
            }
        }
    }
    output[out_pos] = '\0';
    *output_len = out_pos;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    Buffer compressed;
    char decompressed[MAX_INPUT_SIZE];
    size_t decompressed_len;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!compress_data(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100.0);
    
    decompress_data(&compressed, decompressed, &decompressed_len);
    
    if (strcmp(input, decompressed) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed successfully: %s\n", decompressed);
    
    return 0;
}