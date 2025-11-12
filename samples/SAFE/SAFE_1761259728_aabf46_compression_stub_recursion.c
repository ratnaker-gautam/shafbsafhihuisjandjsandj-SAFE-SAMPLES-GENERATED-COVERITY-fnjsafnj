//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = data[start];
        result.length = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = compress_chunk(data, start, mid);
    Buffer right = compress_chunk(data, mid, end);
    
    size_t total_len = left.length + right.length;
    if (total_len > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.length);
    memcpy(result.data + left.length, right.data, right.length);
    result.length = total_len;
    
    return result;
}

int validate_input(const uint8_t* data, size_t length) {
    if (data == NULL) {
        return 0;
    }
    if (length == 0 || length > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void print_buffer(const Buffer* buf) {
    if (buf == NULL) {
        return;
    }
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return 1;
    }
    
    if (input_len % 2 != 0) {
        return 1;
    }
    
    size_t data_len = input_len / 2;
    for (size_t i = 0; i < data_len; i++) {
        char hex[3] = {line[i * 2], line[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (endptr != hex + 2 || value < 0 || value > 255) {
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    if (!validate_input(input, data_len)) {
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, data_len);
    print_buffer(&compressed);
    
    return 0;
}