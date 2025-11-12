//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

size_t recursive_compress(const uint8_t* input, size_t pos, size_t len, uint8_t* output, size_t out_pos) {
    if (pos >= len) {
        return out_pos;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (count > 3 || current == '\\' || current == '\'') {
        if (out_pos + 3 >= MAX_INPUT_SIZE) {
            return out_pos;
        }
        output[out_pos++] = '\\';
        output[out_pos++] = (uint8_t)count;
        output[out_pos++] = current;
    } else {
        for (size_t i = 0; i < count; i++) {
            if (out_pos >= MAX_INPUT_SIZE) {
                return out_pos;
            }
            output[out_pos++] = current;
        }
    }
    
    return recursive_compress(input, pos + count, len, output, out_pos);
}

int compress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->length == 0) {
        return 0;
    }
    
    output->length = recursive_compress(input->data, 0, input->length, output->data, 0);
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->length == 0) {
        return;
    }
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf->length % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    char user_input[MAX_INPUT_SIZE + 1];
    Buffer input_buf = {0};
    Buffer output_buf = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[--input_len] = '\0';
    }
    
    if (!validate_input(user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_len > 0) {
        memcpy(input_buf.data, user_input, input_len);
        input_buf.length = input_len;
        
        if (compress_buffer(&input_buf, &output_buf)) {
            printf("Original size: %zu bytes\n", input_buf.length);
            printf("Compressed size: %zu bytes\n", output_buf.length);
            printf("Compression ratio: %.2f%%\n", 
                   (1.0 - (double)output_buf.length / input_buf.length) * 100);
            printf("Compressed data (hex):\n");
            print_buffer_hex(&output_buf);
        } else {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
    } else {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    return 0;
}