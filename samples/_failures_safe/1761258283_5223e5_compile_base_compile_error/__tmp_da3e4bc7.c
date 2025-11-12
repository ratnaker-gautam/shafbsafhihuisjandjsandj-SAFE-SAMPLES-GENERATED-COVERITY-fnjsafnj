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
    uint8_t flags;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t checksum;
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
    uint32_t output_size;
    uint8_t *compressed_data;
};

void init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->max_blocks = max_blocks;
    ctx->blocks = calloc(max_blocks, sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return;
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

int add_block_to_context(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) {
        return 0;
    }
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->data = malloc(size);
    if (block->data == NULL) {
        return 0;
    }
    memcpy(block->data, data, size);
    block->block_size = size;
    block->checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        block->checksum += data[i];
    }
    ctx->block_count++;
    return 1;
}

struct CompressionResult compress_data(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    if (ctx == NULL || ctx->block_count == 0) {
        return result;
    }
    uint32_t total_size = sizeof(struct CompressionHeader);
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        total_size += sizeof(uint32_t) * 2 + ctx->blocks[i].block_size;
    }
    if (total_size > 1048576) {
        return result;
    }
    uint8_t *output = malloc(total_size);
    if (output == NULL) {
        return result;
    }
    uint8_t *ptr = output;
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = total_size - sizeof(struct CompressionHeader);
    ctx->header.compressed_size = ctx->header.original_size;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    ctx->header.reserved = 0;
    memcpy(ptr, &ctx->header, sizeof(struct CompressionHeader));
    ptr += sizeof(struct CompressionHeader);
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        memcpy(ptr, &ctx->blocks[i].block_size, sizeof(uint32_t));
        ptr += sizeof(uint32_t);
        memcpy(ptr, &ctx->blocks[i].checksum, sizeof(uint32_t));
        ptr += sizeof(uint32_t);
        memcpy(ptr, ctx->blocks[i].data, ctx->blocks[i].block_size);
        ptr += ctx->blocks[i].block_size;
    }
    result.success = 1;
    result.output_size = total_size;
    result.compressed_data = output;
    return result;
}

void cleanup_compression_result(struct CompressionResult *result) {
    if (result != NULL && result->compressed_data != NULL) {
        free(result->compressed_data);
        result->compressed_data = NULL;
    }
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 10);
    if (ctx.blocks == NULL) {
        return 1;
    }
    uint8_t test_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
    uint8_t test_data2[] = {0x57, 0x6F, 0x72, 0x6C, 0x64};
    if (!add_block_to_context(&ctx, test_data1, sizeof(test_data1))) {
        cleanup_compression_context(&ctx);
        return 1;
    }
    if (!add_block_to_context(&ctx, test_data2, sizeof(test_data2))) {
        cleanup_compression_context(&ctx);
        return 1;
    }
    struct CompressionResult result = compress_data(&ctx);
    if (!result.success) {
        cleanup_compression_context(&ctx);
        return 1;
    }