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

int append_to_file_data(struct file_data *fd, const uint8_t *data, size_t len) {
    if (fd == NULL || data == NULL || len == 0) return 0;
    if (fd->size + len > fd->capacity) {
        size_t new_capacity = fd->capacity * 2;
        if (new_capacity < fd->size + len) new_capacity = fd->size + len;
        if (!resize_file_data(fd, new_capacity)) return 0;
    }
    memcpy(fd->data + fd->size, data, len);
    fd->size += len;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2)) return 0;
    
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!append_to_file_data(output, run, 2)) {
            free_file_data(output);
            return 0;
        }
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        uint8_t byte = input->data[i];
        if (!append_to_file_data(output, &byte, 1)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        uint8_t byte = input->data[i];
        if (!append_to_file_data(output, &byte, 1)) {
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
            return init_file_data(output, input->size) && 
                   append_to_file_data(output, input->data, input->size);
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
    printf("3: Huffman\n");
}

int main(void) {
    struct file_data input, output;
    enum compression_type comp_type;
    char input_buffer[1024];
    size_t input_len;
    int choice;
    
    printf("Enter text to compress (max 1023 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_buffer);