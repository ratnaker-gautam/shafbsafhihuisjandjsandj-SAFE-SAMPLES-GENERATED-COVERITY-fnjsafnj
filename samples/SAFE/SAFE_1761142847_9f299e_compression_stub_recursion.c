//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    char data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int compress_chunk(Buffer* input, Buffer* output, size_t pos) {
    if (pos >= input->length) {
        return 0;
    }

    char current = input->data[pos];
    size_t count = 1;
    
    size_t next_pos = pos + 1;
    while (next_pos < input->length && input->data[next_pos] == current && count < 9) {
        count++;
        next_pos++;
    }

    if (count > 1) {
        if (output->length + 2 >= MAX_INPUT_SIZE) {
            return -1;
        }
        output->data[output->length++] = current;
        output->data[output->length++] = '0' + count;
    } else {
        if (output->length + 1 >= MAX_INPUT_SIZE) {
            return -1;
        }
        output->data[output->length++] = current;
    }

    return compress_chunk(input, output, next_pos);
}

int perform_compression(Buffer* input, Buffer* output) {
    output->length = 0;
    return compress_chunk(input, output, 0);
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    printf("Enter text to compress (max %d characters):\n", MAX_INPUT_SIZE - 1);
    
    if (fgets(input.data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input.length = strlen(input.data);
    if (input.length > 0 && input.data[input.length - 1] == '\n') {
        input.data[--input.length] = '\0';
    }
    
    if (input.length == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int result = perform_compression(&input, &output);
    if (result != 0) {
        fprintf(stderr, "Compression failed: output buffer overflow\n");
        return 1;
    }
    
    output.data[output.length] = '\0';
    printf("Compressed: %s\n", output.data);
    
    size_t original_size = input.length;
    size_t compressed_size = output.length;
    printf("Original size: %zu, Compressed size: %zu\n", original_size, compressed_size);
    
    if (compressed_size < original_size) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / original_size) * 100);
    } else {
        printf("No compression achieved\n");
    }
    
    return 0;
}