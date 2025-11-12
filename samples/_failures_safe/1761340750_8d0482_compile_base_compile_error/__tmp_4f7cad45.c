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
    if (ctx == NULL) {
        return;
    }
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
    if (ctx == NULL) {
        return;
    }
    for (uint32_t i = 0; i < ctx->max_blocks; i++) {
        if (ctx->blocks[i].data != NULL) {
            free(ctx->blocks[i].data);
        }
    }
    free(ctx->blocks);
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return -1;
    }
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max <= ctx->max_blocks) {
            return -1;
        }
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) {
            return -1;
        }
        for (uint32_t i = ctx->max_blocks; i < new_max; i++) {
            new_blocks[i].data = NULL;
        }
        ctx->blocks = new_blocks;
        ctx->max_blocks = new_max;
    }
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = 0;
    block->size = size;
    block->compressed_size = size;
    block->data = malloc(size);
    if (block->data == NULL) {
        return -1;
    }
    memcpy(block->data, data, size);
    if (ctx->header.original_size + size < ctx->header.original_size) {
        free(block->data);
        block->data = NULL;
        return -1;
    }
    ctx->header.original_size += size;
    ctx->header.compressed_size += size;
    ctx->block_count++;
    return 0;
}

uint32_t simple_checksum(const uint8_t *data, uint32_t size) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < size; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
    }
    return sum;
}

void finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        ctx->header.checksum ^= simple_checksum(ctx->blocks[i].data, ctx->blocks[i].size);
    }
}

int write_compressed_data(const struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL) {
        return -1;
    }
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }
    if (fwrite(&ctx->header, sizeof(ctx->header), 1, file) != 1) {
        fclose(file);
        return -1;
    }
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (fwrite(&ctx->blocks[i].block_id, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(&ctx->blocks[i].offset, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(&ctx->blocks[i].size, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(&ctx->blocks[i].compressed_size, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(ctx->blocks[i].data, 1, ctx->blocks[i].size, file) != ctx->blocks[i].size) {
            fclose