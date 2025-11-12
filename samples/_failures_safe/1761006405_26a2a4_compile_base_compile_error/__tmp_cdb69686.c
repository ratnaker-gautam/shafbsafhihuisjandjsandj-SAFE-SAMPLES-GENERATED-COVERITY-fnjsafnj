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

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2)) return 0;
    
    for (size_t i = 0; i < input->size; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (output->size + 2 > output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) return 0;
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
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        if (output->size >= output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) return 0;
            output->data = new_data;
            output->capacity = new_cap;
        }
        output->data[output->size++] = input->data[i] ^ 0xAA;
    }
    
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    for (size_t i = 0; i < input->size; i++) {
        if (output->size >= output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) return 0;
            output->data = new_data;
            output->capacity = new_cap;
        }
        output->data[output->size++] = ~input->data[i];
    }
    
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    switch (type) {
        case COMP_NONE:
            if (!init_file_data(output, input->size)) return 0;
            memcpy(output->data, input->data, input->size);
            output->size = input->size;
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

void print_usage(const char *prog_name) {
    printf("Usage: %s <compression_type> <data_string>\n", prog_name);
    printf("Compression types: 0=none, 1=RLE, 2=LZW, 3=Huffman\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    char *endptr;
    long type_num = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || type_num < 0 || type_num > 3) {
        printf("Error: Invalid compression type\n");
        print_usage(argv[0]);
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)type_num;
    const char *input_str = argv[2];
    size_t input_len = strlen(input_str);
    
    if (input_len == 0) {
        printf