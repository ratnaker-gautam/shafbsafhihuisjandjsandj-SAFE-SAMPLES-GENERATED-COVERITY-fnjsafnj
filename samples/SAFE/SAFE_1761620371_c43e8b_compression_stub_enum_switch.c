//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum comp_mode {
    COMPRESS,
    DECOMPRESS,
    INVALID
};

enum comp_mode parse_mode(const char *mode_str) {
    if (strcmp(mode_str, "compress") == 0) {
        return COMPRESS;
    } else if (strcmp(mode_str, "decompress") == 0) {
        return DECOMPRESS;
    } else {
        return INVALID;
    }
}

void compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    size_t in_idx = 0;
    while (in_idx < input_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        while (in_idx + count < input_len && input[in_idx + count] == current && count < 255) {
            count++;
        }
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    *output_len = out_idx;
}

void decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    if (input_len % 2 != 0) {
        fprintf(stderr, "Invalid compressed data length\n");
        exit(EXIT_FAILURE);
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        if (out_idx + count > *output_len) {
            fprintf(stderr, "Output buffer overflow\n");
            exit(EXIT_FAILURE);
        }
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    *output_len = out_idx;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <compress|decompress> <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    enum comp_mode mode = parse_mode(argv[1]);
    if (mode == INVALID) {
        fprintf(stderr, "Invalid mode. Use 'compress' or 'decompress'.\n");
        return EXIT_FAILURE;
    }

    FILE *input_file = fopen(argv[2], "rb");
    if (!input_file) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if (file_size < 0) {
        perror("Failed to get file size");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    size_t input_len = (size_t)file_size;
    uint8_t *input_data = malloc(input_len);
    if (!input_data) {
        perror("Memory allocation failed");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    if (fread(input_data, 1, input_len, input_file) != input_len) {
        perror("Failed to read input file");
        free(input_data);
        fclose(input_file);
        return EXIT_FAILURE;
    }
    fclose(input_file);

    size_t output_len;
    uint8_t *output_data;

    switch (mode) {
        case COMPRESS:
            output_len = input_len * 2;
            output_data = malloc(output_len);
            if (!output_data) {
                perror("Memory allocation failed");
                free(input_data);
                return EXIT_FAILURE;
            }
            compress_data(input_data, input_len, output_data, &output_len);
            break;
        case DECOMPRESS:
            output_len = input_len * 255;
            output_data = malloc(output_len);
            if (!output_data) {
                perror("Memory allocation failed");
                free(input_data);
                return EXIT_FAILURE;
            }
            decompress_data(input_data, input_len, output_data, &output_len);
            break;
        default:
            fprintf(stderr, "Invalid mode\n");
            free(input_data);
            return EXIT_FAILURE;
    }

    FILE *output_file = fopen(argv[3], "wb");
    if (!output_file) {
        perror("Failed to open output file");
        free(input_data);
        free(output_data);
        return EXIT_FAILURE;
    }

    if (fwrite(output_data, 1, output_len, output_file) != output_len) {
        perror("Failed to write output file");
        free(input_data);
        free(output_data);
        fclose(output_file);
        return EXIT_FAILURE;
    }

    free(input_data);
    free(output_data);
    fclose(output_file);

    return EXIT_SUCCESS;
}