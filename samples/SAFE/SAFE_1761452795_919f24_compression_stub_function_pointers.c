//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressFunc)(const Buffer*);
typedef Buffer (*DecompressFunc)(const Buffer*);

Buffer compress_rle(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size > MAX_INPUT_SIZE) {
        return output;
    }
    
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        return output;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && count < UINT8_MAX && 
               input->data[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_idx++] = (uint8_t)count;
        output.data[out_idx++] = current;
        in_idx += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_rle(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || 
        input->size > MAX_INPUT_SIZE || (input->size % 2) != 0) {
        return output;
    }
    
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        return output;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

Buffer process_input(CompressFunc compress, DecompressFunc decompress, const Buffer *input, int mode) {
    if (compress == NULL || decompress == NULL || input == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    if (mode == 0) {
        return compress(input);
    } else if (mode == 1) {
        return decompress(input);
    } else {
        Buffer empty = {NULL, 0};
        return empty;
    }
}

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    int mode;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    printf("Enter mode (0=compress, 1=decompress): ");
    if (scanf("%d", &mode) != 1 || (mode != 0 && mode != 1)) {
        printf("Invalid mode\n");
        return 1;
    }
    
    Buffer input;
    input.data = (uint8_t*)input_str;
    input.size = len;
    
    CompressFunc compress_ptr = compress_rle;
    DecompressFunc decompress_ptr = decompress_rle;
    
    Buffer output = process_input(compress_ptr, decompress_ptr, &input, mode);
    
    if (output.data == NULL || output.size == 0) {
        printf("Processing failed\n");
        if (output.data != NULL) {
            free(output.data);
        }
        return 1;
    }
    
    printf("Result: ");
    print_buffer(&output);
    
    free(output.data);
    return 0;
}