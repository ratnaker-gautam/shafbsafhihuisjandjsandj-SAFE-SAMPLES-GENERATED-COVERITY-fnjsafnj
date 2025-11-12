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
    if (fd == NULL || initial_capacity == 0) return -1;
    fd->data = malloc(initial_capacity);
    if (fd->data == NULL) return -1;
    fd->size = 0;
    fd->capacity = initial_capacity;
    return 0;
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
    if (input == NULL || output == NULL || input->size == 0) return -1;
    if (output->capacity < input->size * 2) return -1;
    
    output->size = 0;
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        if (output->size + 2 > output->capacity) return -1;
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
    return 0;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return -1;
    if (output->capacity < input->size) return -1;
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    return 0;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return -1;
    if (output->capacity < input->size) return -1;
    
    for (size_t i = 0; i < input->size; i++) {
        output->data[i] = input->data[i] ^ 0x55;
    }
    output->size = input->size;
    return 0;
}

int perform_compression(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL) return -1;
    
    switch (type) {
        case COMP_NONE:
            if (output->capacity < input->size) return -1;
            memcpy(output->data, input->data, input->size);
            output->size = input->size;
            return 0;
            
        case COMP_RLE:
            return compress_rle(input, output);
            
        case COMP_LZW:
            return compress_lzw(input, output);
            
        case COMP_HUFFMAN:
            return compress_huffman(input, output);
            
        default:
            return -1;
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
    int type_input;
    char user_input[256];
    
    if (init_file_data(&input, 1024) != 0) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (init_file_data(&output, 2048) != 0) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        free_file_data(&input);
        return 1;
    }
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        free_file_data(&input);
        free_file_data(&output);
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free_file_data(&input);
        free_file_data(&output);
        return 1;
    }
    
    if (input_len > input.capacity) {
        printf("Input too large\n");
        free_file_data(&input);
        free_file_data(&output);
        return 1;
    }
    
    memcpy(input.data, user_input, input_len);
    input.size = input_len;
    
    print_compression_types();
    printf("Select compression type (0-3): ");
    if (scanf("%d", &type_input) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_file_data(&input