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
#define SAFE_ADD(a, b) (((a) > SIZE_MAX - (b)) ? SIZE_MAX : ((a) + (b)))

typedef struct {
    unsigned char data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static void write_byte(Buffer *buf, unsigned char byte) {
    if (buf->size < MAX_OUTPUT_SIZE) {
        buf->data[buf->size] = byte;
        buf->size++;
    }
}

static void write_run(Buffer *buf, unsigned char byte, size_t count) {
    if (count > RLE_THRESHOLD) {
        write_byte(buf, 0xFF);
        write_byte(buf, (unsigned char)(count > 255 ? 255 : count));
        write_byte(buf, byte);
    } else {
        for (size_t i = 0; i < count; i++) {
            write_byte(buf, byte);
        }
    }
}

static int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size > MAX_INPUT_SIZE) {
        return -1;
    }

    output->size = 0;
    if (input_size == 0) {
        return 0;
    }

    unsigned char current = input[0];
    size_t count = 1;

    for (size_t i = 1; i <= input_size; i++) {
        if (i < input_size && input[i] == current && count < 255) {
            count++;
        } else {
            write_run(output, current, count);
            if (i < input_size) {
                current = input[i];
                count = 1;
            }
        }
    }
    return 0;
}

static int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size > MAX_OUTPUT_SIZE) {
        return -1;
    }

    output->size = 0;
    size_t i = 0;

    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            size_t run_length = input[i + 1];
            unsigned char byte = input[i + 2];
            if (run_length == 0) {
                return -1;
            }
            size_t new_size = SAFE_ADD(output->size, run_length);
            if (new_size > MAX_OUTPUT_SIZE || new_size < output->size) {
                return -1;
            }
            for (size_t j = 0; j < run_length; j++) {
                write_byte(output, byte);
            }
            i += 3;
        } else {
            write_byte(output, input[i]);
            i++;
        }
    }
    return 0;
}

static void hex_dump(const unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};

    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen((char *)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    if (compress_rle(input, input_len, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }

    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);

    printf("\nCompressed data (hex):\n");
    hex_dump(compressed.data, compressed.size);

    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }

    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }

    printf("\nDecompressed text: %s\n", decompressed.data);
    return