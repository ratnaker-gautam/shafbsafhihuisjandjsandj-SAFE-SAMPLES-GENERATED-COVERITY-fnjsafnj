//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t compressed_size;
    uint32_t original_size;
    uint8_t algorithm;
    uint8_t flags;
    uint16_t reserved;
};

struct CompressionBlock {
    uint32_t block_offset;
    uint32_t block_size;
    uint32_t compressed_block_size;
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
        return -1;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return -1;
    }
    
    memset(ctx->blocks, 0, max_blocks * sizeof(struct CompressionBlock));
    ctx->max_blocks = max_blocks;
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
    
    return 0;
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
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t data_size) {
    if (ctx == NULL || data == NULL || data_size == 0 || data_size > 65536) {
        return -1;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        return -1;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->data = malloc(data_size);
    if (block->data == NULL) {
        return -1;
    }
    
    memcpy(block->data, data, data_size);
    block->block_size = data_size;
    block->compressed_block_size = data_size;
    block->block_offset = ctx->header.compressed_size;
    
    ctx->header.compressed_size += data_size;
    ctx->header.original_size += data_size;
    ctx->block_count++;
    
    return 0;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->block_count == 0) {
        return -1;
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        if (block->block_size > 4) {
            block->compressed_block_size = block->block_size - 2;
        } else {
            block->compressed_block_size = block->block_size;
        }
    }
    
    return 0;
}

int write_compressed_file(struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || strlen(filename) == 0) {
        return -1;
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }
    
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return -1;
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        if (fwrite(&block->block_offset, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(&block->block_size, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(&block->compressed_block_size, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        if (fwrite(block->data, 1, block->compressed_block_size, file) != block->compressed_block_size) {
            fclose(file);
            return -1;
        }
    }
    
    fclose(file);
    return 0;
}

int main(void) {
    struct CompressionContext ctx;
    
    if (init_compression_context(&ctx, 10) != 0) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t test_data1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t test_data2[] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    uint8_t test_data3[] =