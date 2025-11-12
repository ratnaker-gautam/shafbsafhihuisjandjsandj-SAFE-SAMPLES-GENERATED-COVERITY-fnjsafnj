//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum[16];
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint8_t flags;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    size_t block_count;
    size_t max_blocks;
    uint8_t *input_data;
    size_t input_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->version > 2) return 0;
    if (header->algorithm > 5) return 0;
    if (header->original_size > SIZE_MAX) return 0;
    if (header->compressed_size > SIZE_MAX) return 0;
    return 1;
}

int add_block(struct CompressionContext *ctx, uint32_t block_id, uint32_t offset, uint32_t size, uint8_t flags) {
    if (ctx == NULL) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    if (offset > ctx->input_size) return 0;
    if (size > ctx->input_size - offset) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = block_id;
    block->offset = offset;
    block->size = size;
    block->flags = flags;
    ctx->block_count++;
    return 1;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->input_data == NULL) return 0;
    if (ctx->input_size == 0) return 0;
    
    size_t remaining = ctx->input_size;
    uint32_t offset = 0;
    uint32_t block_id = 0;
    
    while (remaining > 0) {
        uint32_t chunk_size = (remaining > 64) ? 64 : remaining;
        if (!add_block(ctx, block_id, offset, chunk_size, 0)) {
            return 0;
        }
        offset += chunk_size;
        remaining -= chunk_size;
        block_id++;
    }
    
    ctx->header.original_size = ctx->input_size;
    ctx->header.compressed_size = ctx->input_size;
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
    memset(ctx->header.checksum, 0, sizeof(ctx->header.checksum));
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    ctx->block_count = 0;
    ctx->max_blocks = 0;
    ctx->input_size = 0;
}

int main(void) {
    struct CompressionContext ctx = {0};
    char input_buffer[256];
    
    printf("Enter data to compress (max 255 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    ctx.input_size = input_len;
    ctx.input_data = malloc(input_len);
    if (ctx.input_data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    memcpy(ctx.input_data, input_buffer, input_len);
    
    ctx.max_blocks = (input_len + 63) / 64 + 1;
    ctx.blocks = malloc(ctx.max_blocks * sizeof(struct CompressionBlock));
    if (ctx.blocks == NULL) {
        printf("Memory allocation failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (!compress_data(&ctx)) {
        printf("Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Compression completed successfully\n");
    printf("Original size: %lu bytes\n", (unsigned long)ctx.header.original_size);
    printf("Compressed size: %lu bytes\n", (unsigned long)ctx.header.compressed_size);
    printf("Number of blocks: %zu\n", ctx.block_count);
    printf("Compression ratio: %.2f%%\n", 
           (double)ctx.header.compressed_size / ctx.header.original_size * 100);
    
    cleanup_context(&ctx);
    return 0;
}