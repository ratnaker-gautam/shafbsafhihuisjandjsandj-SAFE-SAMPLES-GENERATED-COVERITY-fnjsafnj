//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct compression_params {
    enum compression_type type;
    uint32_t max_size;
    uint8_t quality;
};

static int validate_input(const uint8_t *data, size_t len, const struct compression_params *params) {
    if (data == NULL || params == NULL) return 0;
    if (len == 0 || len > params->max_size) return 0;
    if (params->quality > 100) return 0;
    return 1;
}

static size_t compress_none(const uint8_t *input, size_t len, uint8_t *output, size_t out_len) {
    if (output == NULL || out_len < len) return 0;
    memcpy(output, input, len);
    return len;
}

static size_t compress_rle(const uint8_t *input, size_t len, uint8_t *output, size_t out_len) {
    if (output == NULL || out_len < len * 2) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t count = 1;
        while (i + count < len && count < 255 && input[i] == input[i + count]) {
            count++;
        }
        if (out_idx + 2 > out_len) return 0;
        output[out_idx++] = count;
        output[out_idx++] = input[i];
        i += count;
    }
    return out_idx;
}

static size_t compress_lzw(const uint8_t *input, size_t len, uint8_t *output, size_t out_len) {
    if (output == NULL || out_len < len) return 0;
    uint16_t dict[4096];
    for (int i = 0; i < 256; i++) dict[i] = i;
    uint16_t next_code = 256;
    uint16_t code = input[0];
    size_t out_idx = 0;
    for (size_t i = 1; i < len && next_code < 4096; i++) {
        uint16_t next_char = input[i];
        uint16_t new_code = code * 256 + next_char;
        int found = 0;
        for (int j = 0; j < next_code; j++) {
            if (dict[j] == new_code) {
                code = j;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (out_idx + 2 > out_len) return 0;
            output[out_idx++] = code >> 8;
            output[out_idx++] = code & 0xFF;
            if (next_code < 4096) dict[next_code++] = new_code;
            code = next_char;
        }
    }
    if (out_idx + 2 <= out_len) {
        output[out_idx++] = code >> 8;
        output[out_idx++] = code & 0xFF;
    }
    return out_idx;
}

static size_t compress_huffman(const uint8_t *input, size_t len, uint8_t *output, size_t out_len) {
    if (output == NULL || out_len < len) return 0;
    uint32_t freq[256] = {0};
    for (size_t i = 0; i < len; i++) freq[input[i]]++;
    uint8_t codes[256][32];
    uint8_t lengths[256] = {0};
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            codes[i][0] = i;
            lengths[i] = 8;
        }
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t sym = input[i];
        if (out_idx + 1 > out_len) return 0;
        output[out_idx++] = codes[sym][0];
    }
    return out_idx;
}

static size_t compress_data(const uint8_t *input, size_t len, uint8_t *output, size_t out_len, const struct compression_params *params) {
    if (!validate_input(input, len, params)) return 0;
    switch (params->type) {
        case COMP_NONE:
            return compress_none(input, len, output, out_len);
        case COMP_RLE:
            return compress_rle(input, len, output, out_len);
        case COMP_LZW:
            return compress_lzw(input, len, output, out_len);
        case COMP_HUFFMAN:
            return compress_huffman(input, len, output, out_len);
        default:
            return 0;
    }
}

int main(void) {
    uint8_t input_data[1024];
    uint8_t output_data[2048];
    struct compression_params params;
    params.type = COMP_RLE;
    params.max_size = 1024;
    params.quality = 80;
    printf("Enter data to compress (max 1024 bytes): ");
    fflush(stdout);
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < sizeof(input_data)) {
        input_data[input_len++] = (uint8