//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t offset;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

struct CompressionResult {
    int success;
    uint32_t input_size;
    uint32_t output_size;
    double ratio;
};

void init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->max_blocks = max_blocks;
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks != NULL) {
        memset(ctx->blocks, 0, max_blocks * sizeof(struct CompressionBlock));
    }
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    if (ctx->blocks != NULL) {
        for (uint32_t i = 0; i < ctx->block_count; i++) {
            if (ctx->blocks[i].data != NULL) {
                free(ctx->blocks[i].data);
            }
        }
        free(ctx->blocks);
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int validate_input_data(const uint8_t *data, uint32_t size) {
    if (data == NULL || size == 0 || size > 1000000) {
        return 0;
    }
    return 1;
}

struct CompressionResult compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    struct CompressionResult result = {0};
    if (ctx == NULL || !validate_input_data(input, input_size) || ctx->blocks == NULL) {
        return result;
    }
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    uint32_t remaining = input_size;
    uint32_t block_size = 1024;
    uint32_t total_compressed = 0;
    uint32_t offset = 0;
    while (remaining > 0 && ctx->block_count < ctx->max_blocks) {
        uint32_t current_block_size = (remaining < block_size) ? remaining : block_size;
        ctx->blocks[ctx->block_count].block_size = current_block_size;
        ctx->blocks[ctx->block_count].offset = offset;
        ctx->blocks[ctx->block_count].data = malloc(current_block_size);
        if (ctx->blocks[ctx->block_count].data == NULL) {
            break;
        }
        memcpy(ctx->blocks[ctx->block_count].data, input + offset, current_block_size);
        total_compressed += current_block_size;
        offset += current_block_size;
        remaining -= current_block_size;
        ctx->block_count++;
    }
    if (remaining == 0) {
        ctx->header.compressed_size = total_compressed;
        ctx->header.checksum = 0;
        for (uint32_t i = 0; i < input_size; i++) {
            ctx->header.checksum ^= input[i];
        }
        result.success = 1;
        result.input_size = input_size;
        result.output_size = total_compressed;
        result.ratio = (input_size > 0) ? (double)total_compressed / input_size : 0.0;
    }
    return result;
}

struct CompressionResult decompress_data(struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    struct CompressionResult result = {0};
    if (ctx == NULL || output == NULL || output_size < ctx->header.original_size) {
        return result;
    }
    uint32_t total_decompressed = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data == NULL) {
            continue;
        }
        if (total_decompressed + ctx->blocks[i].block_size > output_size) {
            break;
        }
        memcpy(output + total_decompressed, ctx->blocks[i].data, ctx->blocks[i].block_size);
        total_decompressed += ctx->blocks[i].block_size;
    }
    if (total_decompressed == ctx->header.original_size) {
        result.success = 1;
        result.input_size = ctx->header.compressed_size;
        result.output_size = total_decompressed;
        result.ratio = (ctx->header.compressed_size > 0) ? (double)total_decompressed / ctx->header.compressed_size : 0.0;
    }
    return result;
}

int main() {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 10);
    uint8_t test_data[] = "This is a test string for compression demonstration.";
    uint32_t data_size = sizeof(test_data) - 1;
    struct CompressionResult comp_result = compress_data(&ctx, test_data,