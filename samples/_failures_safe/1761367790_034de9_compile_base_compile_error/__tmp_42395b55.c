//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
    uint8_t checksum;
};

struct DictionaryEntry {
    uint8_t symbol;
    uint32_t frequency;
    uint8_t code_length;
    uint16_t code;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

void init_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x434D5052;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    if (block->dict_size > MAX_DICT_SIZE) return 0;
    return 1;
}

int compress_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0 || input_size > MAX_BLOCK_SIZE) {
        return 0;
    }

    struct CompressionBlock *block = &ctx->current_block;
    block->size = input_size;
    memcpy(block->data, input, input_size);

    block->dict_size = 0;
    for (size_t i = 0; i < input_size && block->dict_size < MAX_DICT_SIZE; i++) {
        uint8_t symbol = input[i];
        int found = 0;
        for (size_t j = 0; j < block->dict_size; j++) {
            if (block->dictionary[j].symbol == symbol) {
                if (block->dictionary[j].frequency < UINT32_MAX) {
                    block->dictionary[j].frequency++;
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            block->dictionary[block->dict_size].symbol = symbol;
            block->dictionary[block->dict_size].frequency = 1;
            block->dictionary[block->dict_size].code_length = 8;
            block->dictionary[block->dict_size].code = (uint16_t)block->dict_size;
            block->dict_size++;
        }
    }

    ctx->header.original_size += (uint32_t)input_size;
    ctx->total_processed += (uint32_t)input_size;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    uint8_t sum = 0;
    const uint8_t *data = (const uint8_t*)&ctx->header;
    for (size_t i = 0; i < sizeof(struct CompressionHeader) - 1; i++) {
        sum ^= data[i];
    }
    ctx->header.checksum = sum;
}

int write_compressed_output(const struct CompressionContext *ctx, FILE *output) {
    if (ctx == NULL || output == NULL) return 0;
    if (!validate_block(&ctx->current_block)) return 0;

    size_t written = fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, output);
    if (written != 1) return 0;

    written = fwrite(&ctx->current_block.size, sizeof(size_t), 1, output);
    if (written != 1) return 0;

    written = fwrite(ctx->current_block.data, 1, ctx->current_block.size, output);
    if (written != ctx->current_block.size) return 0;

    written = fwrite(&ctx->current_block.dict_size, sizeof(size_t), 1, output);
    if (written != 1) return 0;

    if (ctx->current_block.dict_size > 0) {
        written = fwrite(ctx->current_block.dictionary, sizeof(struct DictionaryEntry), ctx->current_block.dict_size, output);
        if (written != ctx->current_block.dict_size) return 0;
    }

    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);

    uint8_t input_data[MAX_BLOCK_SIZE];
    size_t data_size = 0;

    printf("Enter data to compress (max %d bytes): ", MAX_BLOCK_SIZE);
    char buffer[MAX_BLOCK_SIZE * 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }

    if (len > MAX_BLOCK_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }

    data_size = len;
    memcpy(input_data, buffer, data_size);

    if (!compress_block(&ctx, input_data, data_size