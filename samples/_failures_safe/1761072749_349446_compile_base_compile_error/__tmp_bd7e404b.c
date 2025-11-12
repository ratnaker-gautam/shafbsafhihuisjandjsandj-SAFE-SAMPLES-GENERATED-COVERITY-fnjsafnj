//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return 0;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return 0;
    }
    for (uint32_t i = 0; i < max_blocks; i++) {
        ctx->blocks[i].data = NULL;
    }
    ctx->max_blocks = max_blocks;
    return 1;
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

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 10000) {
        return 0;
    }
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    uint32_t index = ctx->block_count;
    ctx->blocks[index].data = malloc(size);
    if (ctx->blocks[index].data == NULL) {
        return 0;
    }
    memcpy(ctx->blocks[index].data, data, size);
    ctx->blocks[index].size = size;
    ctx->blocks[index].offset = (index == 0) ? 0 : ctx->blocks[index - 1].offset + ctx->blocks[index - 1].size;
    ctx->block_count++;
    ctx->header.compressed_size += size;
    return 1;
}

void simulate_compression(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->block_count == 0) {
        return;
    }
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
    ctx->header.original_size = ctx->header.compressed_size * 2;
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        ctx->header.checksum ^= ctx->blocks[i].size & 0xFF;
    }
    ctx->header.flags = 0x01;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Compressed Size: %u\n", ctx->header.compressed_size);
    printf("  Original Size: %u\n", ctx->header.original_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Flags: 0x%02X\n", ctx->header.flags);
    printf("Blocks: %u\n", ctx->block_count);
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("  Block %u: offset=%u, size=%u\n", i, ctx->blocks[i].offset, ctx->blocks[i].size);
    }
}

int main(void) {
    struct CompressionContext ctx;
    if (!init_compression_context(&ctx, 5)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }

    uint8_t sample_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
    uint8_t sample_data2[] = {0x57, 0x6F, 0x72, 0x6C, 0x64};
    uint8_t sample_data3[] = {0x21, 0x0A};

    if (!add_compression_block(&ctx, sample_data1, sizeof(sample_data1))) {
        fprintf(stderr, "Failed to add block 1\n");
        cleanup_compression_context(&ctx);
        return 1;
    }
    if (!add_compression_block(&ctx, sample_data2, sizeof(sample_data2))) {
        fprintf(stderr, "Failed to add block 2\n");
        cleanup_compression_context(&ctx);
        return 1;
    }
    if (!add_compression_block(&ctx, sample_data3, sizeof(sample_data3)))