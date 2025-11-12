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
    if (fd != NULL) {
        free(fd->data);
        fd->data = NULL;
        fd->size = 0;
        fd->capacity = 0;
    }
}

int append_byte(struct file_data *fd, uint8_t byte) {
    if (fd == NULL) return 0;
    if (fd->size >= fd->capacity) {
        size_t new_capacity = fd->capacity * 2;
        if (new_capacity <= fd->capacity) return 0;
        uint8_t *new_data = realloc(fd->data, new_capacity);
        if (new_data == NULL) return 0;
        fd->data = new_data;
        fd->capacity = new_capacity;
    }
    fd->data[fd->size] = byte;
    fd->size++;
    return 1;
}

int compress_none(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL) return 0;
    if (!init_file_data(output, input->size + 1)) return 0;
    output->data[0] = COMP_NONE;
    output->size = 1;
    for (size_t i = 0; i < input->size; i++) {
        if (!append_byte(output, input->data[i])) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2 + 2)) return 0;
    output->data[0] = COMP_RLE;
    output->size = 1;
    
    size_t pos = 0;
    while (pos < input->size) {
        uint8_t current = input->data[pos];
        size_t count = 1;
        while (pos + count < input->size && count < 255 && 
               input->data[pos + count] == current) {
            count++;
        }
        if (!append_byte(output, (uint8_t)count) || !append_byte(output, current)) {
            free_file_data(output);
            return 0;
        }
        pos += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL) return 0;
    if (!init_file_data(output, input->size + 10)) return 0;
    output->data[0] = COMP_LZW;
    output->size = 1;
    
    if (input->size > 0) {
        if (!append_byte(output, (uint8_t)(input->size >> 8)) || 
            !append_byte(output, (uint8_t)(input->size & 0xFF))) {
            free_file_data(output);
            return 0;
        }
        for (size_t i = 0; i < input->size; i++) {
            if (!append_byte(output, input->data[i])) {
                free_file_data(output);
                return 0;
            }
        }
    } else {
        if (!append_byte(output, 0) || !append_byte(output, 0)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL) return 0;
    if (!init_file_data(output, input->size + 20)) return 0;
    output->data[0] = COMP_HUFFMAN;
    output->size = 1;
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input->size; i++) {
        freq[input->data[i]]++;
    }
    
    for (int i = 0; i < 256; i++) {
        if (!append_byte(output, freq[i])) {
            free_file_data(output);
            return 0;
        }
    }
    
    for (size_t i = 0; i < input->size; i++) {
        if (!append_byte(output, input->data[i])) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    switch (type) {
        case COMP_NONE:
            return compress_none(input, output);
        case COMP_RLE:
            return compress_rle(input, output);
        case COMP_LZW:
            return compress_lzw(input,