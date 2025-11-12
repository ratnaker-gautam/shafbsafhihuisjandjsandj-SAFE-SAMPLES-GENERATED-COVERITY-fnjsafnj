//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct file_data {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

int init_file_data(struct file_data *fd, size_t initial_capacity) {
    if (fd == NULL || initial_capacity == 0) return 0;
    fd->data = malloc(initial_capacity);
    if (fd->data == NULL) return 0;
    fd->size = 0;
    fd->capacity = initial_capacity;
    return 1;
}

void free_file_data(struct file_data *fd) {
    if (fd != NULL) {
        free(fd->data);
        fd->data = NULL;
        fd->size = 0;
        fd->capacity = 0;
    }
}

int append_data(struct file_data *fd, const uint8_t *new_data, size_t data_size) {
    if (fd == NULL || new_data == NULL || data_size == 0) return 0;
    
    if (fd->size + data_size > fd->capacity) {
        size_t new_capacity = fd->capacity * 2;
        if (new_capacity < fd->size + data_size) {
            new_capacity = fd->size + data_size;
        }
        uint8_t *new_buffer = realloc(fd->data, new_capacity);
        if (new_buffer == NULL) return 0;
        fd->data = new_buffer;
        fd->capacity = new_capacity;
    }
    
    memcpy(fd->data + fd->size, new_data, data_size);
    fd->size += data_size;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    for (size_t i = 0; i < input->size;) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        uint8_t run[2] = {current, (uint8_t)count};
        if (!append_data(output, run, 2)) return 0;
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint16_t code = (i < input->size - 1) ? 
                       ((uint16_t)input->data[i] << 8) | input->data[i + 1] : 
                       ((uint16_t)input->data[i] << 8);
        
        uint8_t encoded[2] = {(uint8_t)(code >> 8), (uint8_t)(code & 0xFF)};
        if (!append_data(output, encoded, 2)) return 0;
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input->size; i++) {
        freq[input->data[i]]++;
    }
    
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            uint8_t symbol_and_freq[2] = {(uint8_t)i, freq[i]};
            if (!append_data(output, symbol_and_freq, 2)) return 0;
        }
    }
    
    uint8_t eof_marker[2] = {0xFF, 0xFF};
    if (!append_data(output, eof_marker, 2)) return 0;
    
    return 1;
}

int perform_compression(enum compression_type type, const struct file_data *input, struct file_data *output) {
    switch (type) {
        case COMP_NONE:
            return append_data(output, input->data, input->size);
        case COMP_RLE:
            return compress_rle(input, output);
        case COMP_LZW:
            return compress_lzw(input, output);
        case COMP_HUFFMAN:
            return compress_huffman(input, output);
        default:
            return 0;
    }
}

void print_compression_types(void) {
    printf("Available compression types:\n");
    printf("0: No compression\n");
    printf("1: Run-Length Encoding (RLE)\n");
    printf("2: LZW\n");
    printf("3: Huffman coding\n");
}

int main(void) {
    struct file_data input, output;
    enum compression_type comp_type;
    int choice;
    
    printf("Simple File Compression Simulator\n");
    print_compression_types();
    printf("Select compression type (0-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice >