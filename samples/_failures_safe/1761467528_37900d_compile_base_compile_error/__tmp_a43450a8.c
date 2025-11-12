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
        if (output->size + 2 > output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) {
                return 0;
            }
            output->data = new_data;
            output->capacity = new_cap;
        }
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    if (!init_file_data(output, input->size * 2)) {
        return 0;
    }
    for (size_t i = 0; i < input->size; i++) {
        if (output->size >= output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) {
                return 0;
            }
            output->data = new_data;
            output->capacity = new_cap;
        }
        output->data[output->size++] = input->data[i];
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    if (!init_file_data(output, input->size + 8)) {
        return 0;
    }
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input->size; i++) {
        freq[input->data[i]]++;
    }
    memcpy(output->data, freq, 256);
    output->size = 256;
    for (size_t i = 0; i < input->size; i++) {
        if (output->size >= output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) {
                return 0;
            }
            output->data = new_data;
            output->capacity = new_cap;
        }
        output->data[output->size++] = input->data[i];
    }
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    switch (type) {
        case COMP_RLE:
            return compress_rle(input, output);
        case COMP_LZW:
            return compress_lzw(input, output);
        case COMP_HUFFMAN:
            return compress_huffman(input, output);
        case COMP_NONE:
        default:
            if (input == NULL || output == NULL) {
                return 0;
            }
            if (!init_file_data(output, input->size)) {
                return 0;
            }
            memcpy(output->data, input->data, input->size);
            output->size = input->size;
            return 1;
    }
}

int main(void) {
    struct file_data input;
    struct file_data output;
    char buffer[256];
    int choice;
    
    if (!init_file_data(&input, 1024)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    printf("Enter data to compress: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        free_file_data(&input);
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] ==