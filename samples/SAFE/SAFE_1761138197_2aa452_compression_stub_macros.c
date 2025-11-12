//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)

#define RLE_THRESHOLD 3
#define BYTE_MASK 0xFF
#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : ((a) + (b))

typedef struct {
    unsigned char data[MAX_OUTPUT_SIZE];
    size_t length;
} Buffer;

static int validate_input(const unsigned char *input, size_t len) {
    return (input != NULL) && (len > 0) && (len <= MAX_INPUT_SIZE);
}

static void simple_rle_compress(const unsigned char *input, size_t len, Buffer *output) {
    output->length = 0;
    if (len == 0) return;

    size_t i = 0;
    while (i < len) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while ((i + count < len) && (input[i + count] == current) && (count < BYTE_MASK)) {
            count++;
        }

        if (count >= RLE_THRESHOLD) {
            if (output->length + 2 > MAX_OUTPUT_SIZE) break;
            output->data[output->length++] = current;
            output->data[output->length++] = (unsigned char)count;
            i += count;
        } else {
            if (output->length + count > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output->data[output->length++] = current;
            }
            i += count;
        }
    }
}

static int safe_read_input(unsigned char *buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) return 0;
    
    int ch;
    size_t count = 0;
    while (count < max_len - 1) {
        ch = getchar();
        if (ch == EOF || ch == '\n') break;
        buffer[count++] = (unsigned char)ch;
    }
    buffer[count] = '\0';
    return 1;
}

static void write_output(const Buffer *buffer) {
    if (buffer == NULL || buffer->length == 0) return;
    
    for (size_t i = 0; i < buffer->length; i++) {
        putchar(buffer->data[i]);
    }
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter data to compress: ");
    if (!safe_read_input(input, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input);
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    simple_rle_compress(input, input_len, &compressed);
    
    printf("Compressed data: ");
    write_output(&compressed);
    printf("\n");
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.length);
    
    return EXIT_SUCCESS;
}