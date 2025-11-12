//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
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
    return 1;
}

size_t recursive_compress(const uint8_t* input, size_t pos, size_t len, uint8_t* output, size_t out_pos) {
    if (pos >= len) {
        return out_pos;
    }
    
    size_t count = 1;
    while (pos + count < len && input[pos] == input[pos + count] && count < 255) {
        count++;
    }
    
    if (count > 3 || input[pos] == 0xFF) {
        if (out_pos + 2 >= MAX_INPUT_SIZE) {
            return out_pos;
        }
        output[out_pos++] = 0xFF;
        output[out_pos++] = (uint8_t)count;
        output[out_pos++] = input[pos];
    } else {
        for (size_t i = 0; i < count; i++) {
            if (out_pos >= MAX_INPUT_SIZE) {
                return out_pos;
            }
            output[out_pos++] = input[pos];
        }
    }
    
    return recursive_compress(input, pos + count, len, output, out_pos);
}

int compress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->length == 0) {
        return 0;
    }
    
    output->length = recursive_compress(input->data, 0, input->length, output->data, 0);
    return output->length > 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->length == 0) {
        return;
    }
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->length - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer input_buf = {0};
    Buffer output_buf = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input_buf.data, input_str, input_len);
    input_buf.length = input_len;
    
    printf("Original (%zu bytes): ", input_buf.length);
    print_buffer_hex(&input_buf);
    
    if (compress_buffer(&input_buf, &output_buf)) {
        printf("Compressed (%zu bytes): ", output_buf.length);
        print_buffer_hex(&output_buf);
        printf("Compression ratio: %.2f%%\n", 
               (output_buf.length * 100.0) / input_buf.length);
    } else {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    return 0;
}