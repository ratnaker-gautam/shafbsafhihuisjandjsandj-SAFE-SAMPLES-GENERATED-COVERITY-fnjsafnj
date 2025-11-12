//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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
    if (fd != NULL && fd->data != NULL) {
        free(fd->data);
        fd->data = NULL;
        fd->size = 0;
        fd->capacity = 0;
    }
}

int resize_file_data(struct file_data *fd, size_t new_capacity) {
    if (fd == NULL || new_capacity == 0) return 0;
    uint8_t *new_data = realloc(fd->data, new_capacity);
    if (new_data == NULL) return 0;
    fd->data = new_data;
    fd->capacity = new_capacity;
    if (fd->size > new_capacity) fd->size = new_capacity;
    return 1;
}

int append_byte(struct file_data *fd, uint8_t byte) {
    if (fd == NULL) return 0;
    if (fd->size >= fd->capacity) {
        size_t new_capacity = fd->capacity * 2;
        if (new_capacity <= fd->capacity) return 0;
        if (!resize_file_data(fd, new_capacity)) return 0;
    }
    fd->data[fd->size] = byte;
    fd->size++;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->capacity)) return 0;
    
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        if (!append_byte(output, current) || !append_byte(output, (uint8_t)count)) {
            free_file_data(output);
            return 0;
        }
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->capacity)) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        if (!append_byte(output, input->data[i])) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->capacity)) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        if (!append_byte(output, input->data[i] ^ 0x55)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    switch (type) {
        case COMP_NONE:
            if (!init_file_data(output, input->capacity)) return 0;
            for (size_t i = 0; i < input->size; i++) {
                if (!append_byte(output, input->data[i])) {
                    free_file_data(output);
                    return 0;
                }
            }
            return 1;
            
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

void print_hex(const struct file_data *fd) {
    if (fd == NULL || fd->data == NULL) return;
    for (size_t i = 0; i < fd->size; i++) {
        printf("%02X ", fd->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (fd->size % 16 != 0) printf("\n");
}

int main(void) {
    struct file_data input, compressed;
    char buffer[256];
    
    if (!init_file_data(&input, 64)) {
        printf("Failed to initialize input buffer\n");
        return 1;
    }
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {