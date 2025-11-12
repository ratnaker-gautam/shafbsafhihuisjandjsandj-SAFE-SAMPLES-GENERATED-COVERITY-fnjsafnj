//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_LZW,
    COMPRESS_HUFFMAN
};

struct file_data {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

int read_input(struct file_data *fd) {
    if (fd == NULL) return 0;
    
    printf("Enter data size (1-1000): ");
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) return 0;
    if (input_size < 1 || input_size > 1000) return 0;
    
    fd->capacity = input_size + 10;
    fd->data = malloc(fd->capacity);
    if (fd->data == NULL) return 0;
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            free(fd->data);
            fd->data = NULL;
            return 0;
        }
        if (byte_val < 0 || byte_val > 255) {
            free(fd->data);
            fd->data = NULL;
            return 0;
        }
        fd->data[i] = (uint8_t)byte_val;
    }
    fd->size = input_size;
    return 1;
}

void compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->data == NULL) return;
    
    output->capacity = input->size * 2 + 10;
    output->data = malloc(output->capacity);
    if (output->data == NULL) return;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output->capacity) break;
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    output->size = out_idx;
}

void compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->data == NULL) return;
    
    output->capacity = input->size + 100;
    output->data = malloc(output->capacity);
    if (output->data == NULL) return;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (out_idx >= output->capacity) break;
        output->data[out_idx++] = input->data[i];
    }
    output->size = out_idx;
}

void compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->data == NULL) return;
    
    output->capacity = input->size + 50;
    output->data = malloc(output->capacity);
    if (output->data == NULL) return;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (out_idx >= output->capacity) break;
        output->data[out_idx++] = input->data[i] ^ 0x55;
    }
    output->size = out_idx;
}

void print_data(const struct file_data *fd) {
    if (fd == NULL || fd->data == NULL) return;
    
    for (size_t i = 0; i < fd->size; i++) {
        printf("%d ", fd->data[i]);
    }
    printf("\n");
}

int main(void) {
    struct file_data input = {0};
    struct file_data output = {0};
    
    if (!read_input(&input)) {
        printf("Error reading input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input.size);
    print_data(&input);
    
    printf("Select compression type:\n");
    printf("1: None\n2: RLE\n3: LZW\n4: Huffman\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        free(input.data);
        return 1;
    }
    
    enum compression_type comp_type;
    switch (choice) {
        case 1: comp_type = COMPRESS_NONE; break;
        case 2: comp_type = COMPRESS_RLE; break;
        case 3: comp_type = COMPRESS_LZW; break;
        case 4: comp_type = COMPRESS_HUFFMAN; break;
        default: 
            free(input.data);
            printf("Invalid choice\n");
            return 1;
    }
    
    switch (comp_type) {
        case COMPRESS_NONE:
            output.data = malloc(input.size);
            if (output.data != NULL) {
                memcpy(output.data, input.data, input.size);
                output.size = input.size;
            }
            break;
        case COMPRESS_RLE:
            compress_r