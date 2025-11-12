//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

int init_file_data(struct file_data *fd, size_t initial_size) {
    if (fd == NULL || initial_size == 0) return 0;
    fd->data = malloc(initial_size);
    if (fd->data == NULL) return 0;
    fd->size = 0;
    fd->capacity = initial_size;
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

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2)) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output->capacity) {
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) return 0;
            output->data = new_data;
            output->capacity = new_cap;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size + 256)) return 0;
    
    output->size = input->size;
    for (size_t i = 0; i < input->size; i++) {
        output->data[i] = input->data[i] ^ 0x55;
    }
    
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    output->size = input->size;
    for (size_t i = 0; i < input->size; i++) {
        output->data[i] = ~input->data[i];
    }
    
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    switch (type) {
        case COMPRESS_NONE:
            if (!init_file_data(output, input->size)) return 0;
            memcpy(output->data, input->data, input->size);
            output->size = input->size;
            return 1;
            
        case COMPRESS_RLE:
            return compress_rle(input, output);
            
        case COMPRESS_LZW:
            return compress_lzw(input, output);
            
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, output);
            
        default:
            return 0;
    }
}

void print_usage(const char *prog_name) {
    printf("Usage: %s <input_size> <compression_type>\n", prog_name);
    printf("Compression types: 0=none, 1=RLE, 2=LZW, 3=Huffman\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    char *endptr;
    long input_size = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || input_size <= 0 || input_size > 1000000) {
        printf("Error: Invalid input size (1-1000000)\n");
        return 1;
    }
    
    long comp_type = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || comp_type < 0 || comp_type > 3) {
        printf("Error: Invalid compression type (0-3)\n");
        return 1;
    }
    
    struct file_data input, output;
    if (!init_file_data(&input, (size_t)input_size)) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }
    
    srand(42);
    for (size_t i = 0; i < (size_t)input_size; i++) {
        input.data[i] = (uint8_t)(