//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    *output_len = out_idx;
}

void process_compression(Buffer* input, Buffer* output) {
    if (!validate_input(input->data, input->length)) {
        output->length = 0;
        return;
    }
    
    uint8_t temp_output[MAX_OUTPUT_SIZE];
    size_t temp_len = 0;
    
    simple_compress(input->data, input->length, temp_output, &temp_len);
    
    if (temp_len > 0 && temp_len <= MAX_OUTPUT_SIZE) {
        memcpy(output->data, temp_output, temp_len);
        output->length = temp_len;
    } else {
        output->length = 0;
    }
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0 && i + 1 < buf->length) {
            printf("\n");
        } else if (i + 1 < buf->length) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input_buf, output_buf;
    initialize_buffer(&input_buf);
    initialize_buffer(&output_buf);
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t pos = 0;
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE) {
        input_buf.data[pos++] = (uint8_t)c;
    }
    input_buf.length = pos;
    
    if (input_buf.length == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    process_compression(&input_buf, &output_buf);
    
    if (output_buf.length == 0) {
        printf("Compression failed or produced no output.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", output_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)output_buf.length / input_buf.length) * 100.0);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(&output_buf);
    
    return 0;
}