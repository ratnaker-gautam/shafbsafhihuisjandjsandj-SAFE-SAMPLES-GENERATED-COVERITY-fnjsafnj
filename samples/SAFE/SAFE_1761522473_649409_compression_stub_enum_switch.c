//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_HUFFMAN,
    COMPRESS_LZW
};

enum operation {
    OP_COMPRESS,
    OP_DECOMPRESS
};

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0) return 0;
    if (len > 1024) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void compress_rle(const char *input, size_t len) {
    if (len == 0) return;
    size_t pos = 0;
    while (pos < len) {
        char current = input[pos];
        uint8_t count = 1;
        while (pos + count < len && input[pos + count] == current && count < 255) {
            count++;
        }
        printf("%c%c", current, (char)count);
        pos += count;
    }
}

void decompress_rle(const char *input, size_t len) {
    if (len % 2 != 0) return;
    for (size_t i = 0; i < len; i += 2) {
        char ch = input[i];
        uint8_t count = (uint8_t)input[i + 1];
        if (count == 0) return;
        for (uint8_t j = 0; j < count; j++) {
            putchar(ch);
        }
    }
}

void compress_none(const char *input, size_t len) {
    for (size_t i = 0; i < len; i++) {
        putchar(input[i]);
    }
}

void decompress_none(const char *input, size_t len) {
    for (size_t i = 0; i < len; i++) {
        putchar(input[i]);
    }
}

void process_compression(enum compression_type type, enum operation op, const char *input, size_t len) {
    switch (type) {
        case COMPRESS_NONE:
            switch (op) {
                case OP_COMPRESS:
                    compress_none(input, len);
                    break;
                case OP_DECOMPRESS:
                    decompress_none(input, len);
                    break;
            }
            break;
        case COMPRESS_RLE:
            switch (op) {
                case OP_COMPRESS:
                    compress_rle(input, len);
                    break;
                case OP_DECOMPRESS:
                    decompress_rle(input, len);
                    break;
            }
            break;
        case COMPRESS_HUFFMAN:
            printf("Huffman compression not implemented");
            break;
        case COMPRESS_LZW:
            printf("LZW compression not implemented");
            break;
    }
}

int main(void) {
    char input[1025];
    int comp_type, operation;
    
    printf("Enter text to process (max 1024 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Select compression type (0=None, 1=RLE, 2=Huffman, 3=LZW): ");
    if (scanf("%d", &comp_type) != 1) {
        return 1;
    }
    
    printf("Select operation (0=Compress, 1=Decompress): ");
    if (scanf("%d", &operation) != 1) {
        return 1;
    }
    
    if (comp_type < 0 || comp_type > 3 || operation < 0 || operation > 1) {
        printf("Invalid selection\n");
        return 1;
    }
    
    printf("Result: ");
    process_compression((enum compression_type)comp_type, (enum operation)operation, input, len);
    printf("\n");
    
    return 0;
}