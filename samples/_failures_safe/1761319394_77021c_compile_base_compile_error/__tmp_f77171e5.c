//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint32_t checksum;
    uint8_t algorithm;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint32_t compressed_size;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

void init_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->max_blocks = 10;
    ctx->blocks = malloc(ctx->max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < ctx->max_blocks; i++) {
        ctx->blocks[i].data = NULL;
    }
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data != NULL) {
            free(ctx->blocks[i].data);
        }
    }
    free(ctx->blocks);
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max <= ctx->max_blocks) return 0;
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) {
            return 0;
        }
        ctx->blocks = new_blocks;
        for (uint32_t i = ctx->max_blocks; i < new_max; i++) {
            ctx->blocks[i].data = NULL;
        }
        ctx->max_blocks = new_max;
    }
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = 0;
    block->size = size;
    block->compressed_size = size;
    block->data = malloc(size);
    if (block->data == NULL) {
        return 0;
    }
    memcpy(block->data, data, size);
    ctx->header.original_size += size;
    ctx->header.compressed_size += size;
    ctx->block_count++;
    return 1;
}

void simulate_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    ctx->header.compressed_size = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        uint32_t compressed = block->size - (block->size / 4);
        if (compressed < block->size) {
            block->compressed_size = compressed;
        } else {
            block->compressed_size = block->size;
        }
        ctx->header.compressed_size += block->compressed_size;
    }
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        ctx->header.checksum ^= ctx->blocks[i].block_id;
        ctx->header.checksum ^= ctx->blocks[i].size;
    }
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    printf("Compression Results:\n");
    printf("Magic: 0x%08X\n", ctx->header.magic);
    printf("Version: %u\n", ctx->header.version);
    printf("Original Size: %lu bytes\n", (unsigned long)ctx->header.original_size);
    printf("Compressed Size: %lu bytes\n", (unsigned long)ctx->header.compressed_size);
    double ratio = 0.0;
    if (ctx->header.original_size > 0) {
        ratio = (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100.0;
    }
    printf("Compression Ratio: %.2f%%\n", ratio);
    printf("Checksum: 0x%08X\n", ctx->header.checksum);
    printf("Blocks: %u\n", ctx->block_count);
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("  Block %u: %u -> %u bytes\n", ctx->blocks[i].block_id, ctx->blocks[i].size, ctx->blocks[i].compressed_size);
    }
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    uint8_t sample_data1[] = {0x41, 0x