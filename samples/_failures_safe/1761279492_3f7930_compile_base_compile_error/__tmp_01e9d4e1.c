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
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->max_blocks = 16;
    ctx->blocks = malloc(ctx->max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        exit(EXIT_FAILURE);
    }
    memset(ctx->blocks, 0, ctx->max_blocks * sizeof(struct CompressionBlock));
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data != NULL) {
            free(ctx->blocks[i].data);
        }
    }
    free(ctx->blocks);
    memset(ctx, 0, sizeof(*ctx));
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 1048576) {
        return 0;
    }
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max <= ctx->max_blocks) {
            return 0;
        }
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) {
            return 0;
        }
        ctx->blocks = new_blocks;
        memset(&ctx->blocks[ctx->max_blocks], 0, (new_max - ctx->max_blocks) * sizeof(struct CompressionBlock));
        ctx->max_blocks = new_max;
    }
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
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

uint32_t compute_checksum(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return 0;
    }
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        for (uint32_t j = 0; j < block->size; j++) {
            checksum = (checksum << 3) | (checksum >> 29);
            checksum ^= block->data[j];
        }
    }
    return checksum;
}

int write_compressed_file(const struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || ctx->block_count == 0) {
        return 0;
    }
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    struct CompressionHeader header = ctx->header;
    header.checksum = compute_checksum(ctx);
    if (fwrite(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        if (fwrite(&block->block_id, sizeof(block->block_id), 1, file) != 1) {
            fclose(file);
            return 0;
        }
        if (fwrite(&block->compressed_size, sizeof(block->compressed_size), 1, file) != 1) {
            fclose(file);
            return 0;
        }
        if (fwrite(block->data, 1, block->compressed_size, file) != block->compressed_size) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    uint8_t test_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    uint8_t test_data2[] = {0x54, 0x65, 0x73, 0x74,