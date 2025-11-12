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

int init_file_data(struct file_data *fd, size_t initial_size) {
    if (fd == NULL || initial_size == 0) {
        return 0;
    }
    fd->data = malloc(initial_size);
    if (fd->data == NULL) {
        return 0;
    }
    fd->size = 0;
    fd->capacity = initial_size;
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
    if (fd == NULL || new_data == NULL || data_size == 0) {
        return 0;
    }
    if (fd->size + data_size > fd->capacity) {
        size_t new_capacity = fd->capacity * 2;
        if (new_capacity < fd->size + data_size) {
            new_capacity = fd->size + data_size;
        }
        uint8_t *new_buffer = realloc(fd->data, new_capacity);
        if (new_buffer == NULL) {
            return 0;
        }
        fd->data = new_buffer;
        fd->capacity = new_capacity;
    }
    memcpy(fd->data + fd->size, new_data, data_size);
    fd->size += data_size;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    if (!init_file_data(output, input->size * 2)) {
        return 0;
    }
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!append_data(output, run, 2)) {
            free_file_data(output);
            return 0;
        }
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    if (!init_file_data(output, input->size)) {
        return 0;
    }
    for (size_t i = 0; i < input->size; i++) {
        uint8_t byte = input->data[i];
        if (!append_data(output, &byte, 1)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    if (!init_file_data(output, input->size)) {
        return 0;
    }
    for (size_t i = 0; i < input->size; i++) {
        uint8_t byte = input->data[i];
        if (!append_data(output, &byte, 1)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_data(enum compression_type method, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    switch (method) {
        case COMP_RLE:
            return compress_rle(input, output);
        case COMP_LZW:
            return compress_lzw(input, output);
        case COMP_HUFFMAN:
            return compress_huffman(input, output);
        case COMP_NONE:
            if (!init_file_data(output, input->size)) {
                return 0;
            }
            return append_data(output, input->data, input->size);
        default:
            return 0;
    }
}

int main(void) {
    struct file_data input_data;
    struct file_data output_data;
    char input_buffer[256];
    int method_choice;
    
    printf("Enter compression method (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    if (sscanf(input_buffer, "%d", &method_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (method_choice < 0 || method_choice > 3) {
        printf("Invalid method\n");
        return 1;
    }
    
    printf("Enter data to compress: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n